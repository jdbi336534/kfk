/*******************************************************************************
 * Copyright (c) 2012, 2013 Beidouapp Corp.
 *
 * All rights reserved. 
 *
 * Contributors:
 *    Peter Peng - initial contribution
 *******************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "esp_common.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"
#include "driver/uart.h"
#include "user_config.h"
#include "et_client.h"
#include "lwip/netdb.h"
#include "et_api_compatible.h"
#include "factory.h"
#include "driver/gpio.h"

#define MAX_USER_ID 32
#define MAX_ILINK_CONNECT  5

et_char g_user_id[ET_USER_ID_LEN_MAX] = {0};
et_char g_group_id[ET_USER_ID_LEN_MAX] = {0};

extern et_int32 read_uart_data_flag;
extern xQueueHandle xQueueUart;
et_cloud_handle g_cloud_handle = NULL;
et_cloud_connect_type g_cloud_con_para = {ET_FALSE, ET_TRUE, 90};
et_uchar kick_out=0; 

struct ilink_connect_client
{
	et_int32 state;
	et_char user_id[MAX_USER_ID];

};

static struct ilink_connect_client g_ilink_connect_clients[MAX_ILINK_CONNECT];
et_int32 to_stop_app = 0;

void print_hex(et_uchar *label, et_uchar *str, et_int32 len)
{
	et_int32 i;
	
	os_printf("%s : ", label);
	for(i = 0; i < len; i++)
		os_printf("%02x ", str[i]);
	os_printf("\n");
}

et_uchar check_sum(et_uchar *buf, et_int32 len)
{
	et_uchar bcc=0;
	et_int32 i;

	for(i = 0; i < len; i ++)
		bcc ^= buf[i];

	return bcc; 
}

static et_int32 ack_to_mqtt(et_uchar *msg_buf, et_uchar *ack_buf, et_int32 rc)
{
	et_int32 len=START_LEN;
	
	ack_buf[0] = 0xFF;
	ack_buf[1] = 0xFF;
	ack_buf[2] = 0x00;
	ack_buf[3] = ACK_ERROR_LEN;
	ack_buf[4] = msg_buf[4] | 0x0F;
	ack_buf[5] = msg_buf[5];
	ack_buf[6] = msg_buf[6];
	ack_buf[7] = rc;
	ack_buf[8] = check_sum(&ack_buf[2], ACK_ERROR_LEN + 1);
	len += ACK_ERROR_LEN;
	
	return len;
}

et_int32 parse_msg_from_mqtt(et_uchar *msg_buf, et_int32 data_len)
{
	et_int32 i, pos=0, rc = -1;
	et_uchar cmd, type, bcc;
	et_int32 len, seq;
	WORK_MODE_T mode;
	msg_to_net_t *msg=NULL;

	#ifdef USER_PRINT_DEBUG
	print_hex("msg_buf", msg_buf, data_len);
	#endif

	if(msg_buf[pos] != 0xFF ||msg_buf[pos + 1] != 0xFF) 
	{
		os_printf("parse packet head error\n");
		return rc;
	}
	pos += 2;
	
	len = (msg_buf[pos] << 8) | msg_buf[pos + 1];
	if(len < 3 || len != data_len - 2 - 2) 
	{
		os_printf("parse packet length error\n");
		return rc;
	}

	msg = (msg_to_net_t *)malloc(sizeof(msg_to_net_t));
	if(msg == NULL) 
	{
		os_printf("malloc msg error\n");
		return rc;
	}	
	
	bcc = check_sum(&msg_buf[pos], len + 2 - 1);
	if(bcc != msg_buf[data_len - 1]) 
	{
		msg->len = ack_to_mqtt(msg_buf, msg->buf, ACK_BCC_ERROR);
		os_printf("bcc error\n");
		et_chat_to(g_cloud_handle,msg->buf, msg->len, g_user_id, SEND_TO_CLOUD);
		free(msg);
		return rc;
	}
	
	pos += 2;
	
	cmd = msg_buf[pos];
	pos += 1;
	
	seq += msg_buf[pos];
	pos += 1;

	type = msg_buf[pos];
	pos += 1;	

	switch(cmd) 
	{
		case CMD_CONTROL: 
		{
			switch(type) 
			{
				case TYPE_RGB_LIGHT_DEV: 
				{
					et_uchar red;
					et_uchar gre;
					et_uchar blu;
				
					red = msg_buf[pos];
					pos += 1;
					gre = msg_buf[pos];
					pos += 1;
					blu = msg_buf[pos];
					os_printf("RGB_light is controllered\n");
				 	RGB_light_set_color(red, gre, blu);      //set rgb color
					msg->len = ack_to_mqtt(msg_buf, msg->buf, ACK_SUCCESS);
					rc = msg->len;
				}
					break;
				case TYPE_MY_SWITCH:
				{
					et_uchar switchval;
					switchval= msg_buf[pos];
					printf("set switch: R(%02x)\r\n", switchval);
					if(switchval== 0xff){   
  						GPIO_OUTPUT_SET(GPIO_ID_PIN(5), 0);
					}else if(switchval==0x00){
						GPIO_OUTPUT_SET(GPIO_ID_PIN(5), 1);
					}
					// SWITCH_ON;
					// SWITCH_OFF;
				}
					break;
				default:
					break;
			}
		}
			break;

		default:
			msg->len = ack_to_mqtt(msg_buf, msg->buf, ACK_CMD_ILLIGAL);
			rc = msg->len;
			break;
	}

	et_chat_to(g_cloud_handle,msg->buf, msg->len, g_user_id, SEND_TO_CLOUD);
	free(msg);
	
	return rc;
}

et_int32 add_userid_to_clients(et_char *userid)
{
	et_int32 i = 0,j = 0;
	et_int32 ret = -1;
	et_char  *str = "now device has max users";
	//et_int32 add_flag = 0;
	
	for(i = 0; i < MAX_ILINK_CONNECT;i++)
	{
		if(g_ilink_connect_clients[i].state == 1)
		{
			ret = strcmp(userid,g_ilink_connect_clients[i].user_id);
			if(ret == 0)
			{
				return -1;
			}
		}
		
	}
			
	for(j = 0; j < MAX_ILINK_CONNECT;j ++)
	{
		if(g_ilink_connect_clients[j].state == -1)
		{
			g_ilink_connect_clients[j].state = 1;
			strcpy(g_ilink_connect_clients[j].user_id,userid);
			os_printf("now add userid is %s\n",g_ilink_connect_clients[j].user_id);
			break;

		}

	}

	if(j >= MAX_ILINK_CONNECT)
	{
		et_chat_to(g_cloud_handle,str,strlen(str),userid,SEND_TO_CLOUD);
		return -1;
	}
	else
	{
		return 0;
	}
		

}

et_int32 remove_userid_clients(et_char *userid)
{
	et_int32 i = 0;
	et_int32 ret = -1;
	for(i = 0; i < MAX_ILINK_CONNECT;i++)
	{
		if(g_ilink_connect_clients[i].state == 1)
		{
			ret = strcmp(userid,g_ilink_connect_clients[i].user_id);
			if(ret == 0)
			{
				os_printf("now userid is %s\n",g_ilink_connect_clients[i].user_id);
				g_ilink_connect_clients[i].state = -1;
				memset(g_ilink_connect_clients[i].user_id,0,sizeof(g_ilink_connect_clients[i].user_id));
				os_printf("now status is %d\n",g_ilink_connect_clients[i].state);
				return 0;
			}
		}
		
	}

	return -1;

}

void init_clients(void)
{
	et_int32 j = 0;
	
	for(j = 0; j < MAX_ILINK_CONNECT;j ++)
	{
		g_ilink_connect_clients[j].state = -1;
		memset(g_ilink_connect_clients[j].user_id,0,sizeof(g_ilink_connect_clients[j].user_id));
			
	}

}

void et_message_process(et_int32 type, et_char *send_userid, et_char *topic_name, et_int32 topic_len, et_context_mes_type *message)
{
	et_char server_ip[32];
	int rc = -1;
	
	switch(type)
	{
		case MES_CHAT_TO_CHAT:
			os_printf("chat mes from:%s\n", send_userid);
			memset(g_user_id, 0, sizeof(g_user_id));
			snprintf(g_user_id, sizeof(g_user_id), "%s",send_userid);
				
			parse_msg_from_mqtt(message->payload, message->payload_len);
			break;
		
		case MES_FROM_GROUP_SUBTOPIC:
			os_printf("Group mes from:%s\n", topic_name);
			memset(g_user_id, 0, sizeof(g_user_id));
			snprintf(g_user_id, sizeof(g_user_id), "%s",send_userid);
			parse_msg_from_mqtt(message->payload, message->payload_len);
			break;
			
		case MES_FROM_LOCAL:
			os_printf("Local mes from %s:%s\n", send_userid, topic_name);
			memset(g_user_id, 0, sizeof(g_user_id));
			snprintf(g_user_id, sizeof(g_user_id), "%s",send_userid);

			parse_msg_from_mqtt(message->payload, message->payload_len);
			break;
		case MES_NOTICE_ADD_BUDDY:
			os_printf("add buddy by %s:%s\n", send_userid, topic_name);
			break;
		case MES_NOTICE_REMOVE_BUDDY:
			os_printf("remove buddy by %s:%s\n", send_userid, topic_name);
			break;
		case MES_USER_OFFLINE:
			os_printf("%s Offline:%s\n", send_userid, topic_name);
			break;
		case MES_USER_ONLINE:
			os_printf("%s Online:%s\n", send_userid, topic_name);
			break;
		case MES_USER_STATUS:
			os_printf("%s Status:%s\n", send_userid, topic_name);
			break;
		case MES_ONLINE_BUDDIES_LIST:
			os_printf("Get online buddies list%s:%s\n", send_userid, topic_name);
			break;
	}
}

void et_event_process(et_event_type event)
{
	et_int32 rc = -1;
	
	switch(event.event_no)
	{
		case EVENT_CLOUD_CONNECT:
			disarm_wifi_spark_timer();
			wifi_led_on();
			os_printf("connected to cloud.\n");
			break;
		case EVENT_CLOUD_DISCONNECT:
			set_wifi_spark_timer(WIFI_LED_TIME_SERVER_CONNECT);
			os_printf("disconnect from cloud.\n");
			//et_logout_cloud(g_cloud_handle);
			//et_stop_srv(g_cloud_handle);
			break;
		case EVENT_LOGIN_KICK:
			kick_out = 1;
			os_printf("logined in other place, kicked out from cloud.\n");
			break;
		case EVENT_CLOUD_SEND_FAILED:
			os_printf("Cloud send failed\n");
			break;
		case EVENT_CLOUD_SEND_SUCCESS:
			os_printf("Cloud send success\n");
			break;
		case EVENT_LOCAL_SEND_FAILED:
			os_printf("Local send failed\n");
			break;
		case EVENT_LOCAL_SEND_SUCCESS:
			os_printf("Local send success\n");
			break;
	}
}

void et_ilink_task(void *pvParameters)
{
	while(1)
	{
		if(!kick_out)
			et_ilink_loop(g_cloud_handle);
		taskYIELD();
	}

	vTaskDelete(NULL);	
}

void et_local_task(void *pvParameters)
{
	while(1)
	{
		et_server_loop(g_cloud_handle);
		taskYIELD();
	}
	
	vTaskDelete(NULL);	
}

void  et_user_main(void *pvParameters)
{
	et_int32 rc = -1;
	id_info_t id;
	et_server_info_t server_info;
	et_net_addr_type g_cloud_addr_para = {NULL, 0};
	
	os_printf("ET U-SDK var%s\n",et_sdk_ver());
	to_stop_app = 0;
	
	memset(&id, 0, sizeof(id_info_t));
	if(get_uid(&id) != SPI_FLASH_RESULT_OK)
	{
		os_printf("et_user_main, get_uid error\n");
		return ;
	}
	
	id.uid[UID_LEN - 1] = '\0';
	id.appkey[APPKEY_LEN - 1] = '\0';
	id.secretkey[SECRETKEY_LEN - 1] = '\0';

	memset(&server_info, 0, sizeof(et_server_info_t));
	if(get_server(&server_info) != SPI_FLASH_RESULT_OK)
	{
		os_printf("et_user_main, get_server error\n");
		return ;
	}

	server_info.lb_addr[ET_LB_ADDR_MAX - 1] = '\0';
	server_info.lb_port[ET_LB_PORT_MAX - 1] = '\0';
	
	g_cloud_addr_para.name_ip = server_info.lb_addr;
	g_cloud_addr_para.port = atoi(server_info.lb_port);

	os_printf("uid : %s\n", id.uid);
	os_printf("appkey : %s\n", id.appkey);
	os_printf("secretkey : %s\n", id.secretkey);
	os_printf("server information %s : %s\n", server_info.lb_addr, server_info.lb_port);

	g_cloud_handle = et_create_context(id.uid, id.appkey, id.secretkey, g_cloud_addr_para);
	if(NULL == g_cloud_handle)
	{
		os_printf("Init et account failed\n");
		goto error;
	}
	
	if(et_set_callback(g_cloud_handle,et_message_process, et_event_process) < 0)
	{
		os_printf("%s et_set_callback failed.\n");
		et_destroy_context(g_cloud_handle);	
		goto error;
	}

	rc = et_start_svr(g_cloud_handle);
	if(rc != 0){
		os_printf("%s et_start_svr fail.\n", __FUNCTION__);
		et_destroy_context(g_cloud_handle);
		goto error;
	}
	
	if(pdPASS != xTaskCreate(et_local_task, "local_task", 768, NULL, 2, NULL))
	{
		os_printf("%s et_local_task failed.\n", __FUNCTION__);
		et_stop_srv(g_cloud_handle);
		et_destroy_context(g_cloud_handle);
		goto error;
	}

	do
	{
		rc = et_login_cloud(g_cloud_handle, g_cloud_con_para);
		if(rc != 0)
		{
			os_printf("login_cloud fail\n");
		}
		vTaskDelay(2000 / portTICK_RATE_MS);
	}while(rc != 0 && to_stop_app == 0);
	
	if(pdPASS != xTaskCreate(et_ilink_task, "ilink_task", 768, NULL, 2, NULL))
	{
		os_printf("%s et_ilink_task failed.\n", __FUNCTION__);
		et_logout_cloud(g_cloud_handle);
		et_destroy_context(g_cloud_handle);
		goto error;
	}

error:
	vTaskDelete(NULL);
	return ;
}


