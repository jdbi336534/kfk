/******************************************************************************
 * Copyright 2013-2014 Espressif Systems (Wuxi)
 *
 * FileName: user_main.c
 *
 * Description: entry file of user application
 *
 * Modification history:
 *     2014/12/1, v1.0 create this file.
*******************************************************************************/
#include "esp_common.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "lwip/sockets.h"
#include "lwip/dns.h"
#include "lwip/netdb.h"
#include "driver/uart.h"
#include "espressif/esp_system.h"
#include "et_types.h"
#include "driver/RGB_light.h"
#include "driver/delay.h"
#include "user_config.h"
#include "driver/gpio.h"
#include "espressif/smartconfig.h"
#include "et_api_compatible.h"
#include "factory.h"

extern et_cloud_handle g_cloud_handle;
extern et_int32 to_stop_app;
et_int8 air_kiss_start_flag = 0;
et_int8 user_main_start_flag = 0;
et_int8 wifi_reconnect_start_flag = 0;

LOCAL os_timer_t test_timer;
LOCAL WORK_MODE_T work_mode = WORK_MODE_RGB;

/******************************************************************************
 * FunctionName : user_esp_platform_check_ip
 * Description  : check whether get ip addr or not
 * Parameters   : none
 * Returns      : none
*******************************************************************************/
void ICACHE_FLASH_ATTR
check_ip(void)
{	
	static et_uint32 time = 0;
	et_uint32 connect_error_flag=0;
	struct ip_info ipconfig;
	et_uchar status=0;
	
	os_timer_disarm(&test_timer);	
	wifi_get_ip_info(STATION_IF, &ipconfig);
	status = wifi_station_get_connect_status();
	
	if (status == STATION_GOT_IP && ipconfig.ip.addr != 0) 
	{
		os_printf("got ip !!! \r\n");
		if (user_main_start_flag == 0)
		{
			user_main_start_flag = 1;
			xTaskCreate(et_user_main, "et_user_main", 512, NULL, 2, NULL);
		}
		wifi_reconnect_start_flag = 1;
	} 
	else 
	{ 
		if(status == STATION_WRONG_PASSWORD)
		{
			if ((system_get_time() - time) >= 5000000)
			{				
				os_printf("connect fail, wrong password!!! \r\n");
				time = system_get_time();
			}
			connect_error_flag = 1;		
		}
		else if(status == STATION_NO_AP_FOUND)
		{
			if ((system_get_time() - time) >= 5000000)
			{				
				os_printf("connect fail, no AP found!!! \r\n");
				time = system_get_time();
			}
			connect_error_flag = 1;	
		}
		else if(status == STATION_CONNECT_FAIL)
		{
			if ((system_get_time() - time) >= 5000000)
			{				
				os_printf("connect fail, connect fail!!! \r\n");
				time = system_get_time();
			}
			connect_error_flag = 1;	
		}

		if(connect_error_flag == 1)
		{
		       if (air_kiss_start_flag == 1)
		       {
			     wifi_station_set_reconnect_policy(false);
			     smartconfig_stop();
			     air_kiss_start_flag = 0;
		       }
		}
		
		//re-arm timer to check ip
		os_timer_setfn(&test_timer, (os_timer_func_t *)check_ip, NULL);
		os_timer_arm(&test_timer, 1000, 0);
	}
}

LOCAL void 
audio_key_init(void)
{
	gpio16_input_conf();
}

void ICACHE_FLASH_ATTR
smartconfig_done(sc_status status, void *pdata)
{
	switch(status) 
	{
		case SC_STATUS_WAIT:
	    		os_printf("smartconfig wait.\n");
	    		break;
				
		case SC_STATUS_FIND_CHANNEL:
			set_wifi_spark_timer(WIFI_LED_TIME_AIRKISS);
	    		os_printf("smartconfig findding channel.\n");
	    		break;
				
		case SC_STATUS_GETTING_SSID_PSWD:
	    		os_printf("smartconfig getting ssid and passwd.\n");
				
	    		sc_type *type = pdata;
		    	if (*type == SC_TYPE_ESPTOUCH) 
			{
		      		os_printf("smartconfig type:type esptouch.\n");
		    	} 
			else 
			{
		        	os_printf("smartconfig type:type airkiss.\n");
			}
			break;
			
		case SC_STATUS_LINK: 
		{
			os_printf("smartconfig linking.\n");
			struct station_config *sta_conf = pdata;

			wifi_station_set_config(sta_conf);
			wifi_station_disconnect();
			wifi_station_connect();
		}
			break;
			
		case SC_STATUS_LINK_OVER: {
			os_printf("smartconfig link over.\n");
	
			smartconfig_stop();
			delay_s(2);
			system_restart();
			break;
		}
	}

}

void airkiss_key_init(key_gpio_t*key)
{
	et_uint32 io_reg;

	io_reg = GPIO_PIN_REG(key->key_num);

	PIN_PULLUP_EN(io_reg);
	PIN_FUNC_SELECT(io_reg, 0);
	GPIO_AS_INPUT(key->key_gpio_pin);
}

void ICACHE_FLASH_ATTR
airkiss_key_poll_task(void *pvParameters)
{
	et_uint32 value, i;
	
	while(1) 
	{
		value = gpio_get_value(AIRKISS_KEY_IO_NUM);
		if(!air_kiss_start_flag && !value) 
		{
			delay_s(1);
			value = gpio_get_value(AIRKISS_KEY_IO_NUM);
			if(!air_kiss_start_flag && !value) 
			{
				os_printf("begin to airkiss\n");
				air_kiss_start_flag = 1;
				os_timer_disarm(&test_timer);
				to_stop_app = 1;  	//in airkiss mode, stop et_user_main thread
				if(g_cloud_handle != NULL)
				{
					et_stop_srv(g_cloud_handle);					et_logout_cloud(g_cloud_handle);
					et_destroy_context(g_cloud_handle);
					g_cloud_handle = NULL;
				}
				delay_s(1);
				wifi_reconnect_start_flag = 0;
				smartconfig_start(smartconfig_done); 	//airkiss start
			
   				os_timer_setfn(&test_timer, (os_timer_func_t *)check_ip, NULL);
   				os_timer_arm(&test_timer, 1000, 0);
			}
		}
		delay_ms(500);
	}

	os_printf("end airkiss\n");
	vTaskDelete(NULL);
}

void et_wifi_event_cb(System_Event_t *event)
{
	switch(event->event_id)
	{
		//ESP8266 station finish scanning AP
		case EVENT_STAMODE_SCAN_DONE:
			break;

		//ESP8266 station connected to AP
		case EVENT_STAMODE_CONNECTED:
			os_printf("connect to ssid %s\n", event->event_info.connected.ssid);
			break;

		//ESP8266 station disconnected to AP
		case EVENT_STAMODE_DISCONNECTED:
		{
			disarm_wifi_spark_timer();
			wifi_led_off();
			if(true != wifi_station_get_reconnect_policy())
				wifi_station_set_reconnect_policy(true);
	
			if(wifi_reconnect_start_flag != 1)
			{
				os_printf("airkiss running or system start first, do not reboot.\n");
			}
			else
			{
				os_printf("wifi connect status %d, restart system\n",wifi_station_get_connect_status());
				system_restart();
			}
		}
			break;

		//ESP8266 station got IP from connected AP
		case EVENT_STAMODE_GOT_IP:
			set_wifi_spark_timer(WIFI_LED_TIME_SERVER_CONNECT);
			break;

		default:
			break;
	}
}

/******************************************************************************
 * FunctionName : user_init
 * Description  : entry of user application, init user function here
 * Parameters   : none
 * Returns      : none
*******************************************************************************/
void user_init(void)
{	
	et_uchar result=0;
	
	os_printf("software version:%s\n", SOFTWARE_VERSION);
	RGB_light_init();
	// init my switch start GPIO4
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTMS_U,FUNC_GPIO5);
	GPIO_OUTPUT_SET(GPIO_ID_PIN(5), 1);
	// init my switch end
	if(get_fac_norm_mode(&result) != SPI_FLASH_RESULT_OK)
	{
		os_printf("get_fac_norm_mode error, NORMAL mode\n");
	}
	if(result == FAC_MODE)
	{
		os_printf("run in factory mode\n");
		RGB_light_set_color(0x00, 0x00, 0xff);
		uart_init_new_uart1(BIT_RATE_115200);
		UART_SetPrintPort(UART1);
		uart_init_new(BIT_RATE_115200);
		return;
	}

	os_printf("run in normal mode\n");
	key_gpio_t key;
	struct station_config config;
	struct ip_info info;	
	
	//wifi event handle
	wifi_set_event_handler_cb(et_wifi_event_cb);
	
	memset(&key, 0, sizeof(key_gpio_t));
	key.key_gpio_pin = AIRKISS_KEY_IO_PIN;
	key.key_num = AIRKISS_KEY_IO_NUM;

	airkiss_key_init(&key);
	wifi_set_opmode(STATION_MODE);
	wifi_reconnect_start_flag = 0;

	// xTaskCreate(airkiss_key_poll_task, "smartconfig_task", 256, NULL, 2, NULL);
	memset(&config, 0, sizeof(struct station_config));
	if(wifi_station_get_config_default(&config) == true) 
	{
		os_printf("ssid=%s\n", config.ssid);
		wifi_station_set_config_current(&config);
	}
	
	os_timer_disarm(&test_timer);
   	os_timer_setfn(&test_timer, (os_timer_func_t *)check_ip, NULL);
   	os_timer_arm(&test_timer, 1000, 0);

}
