#ifndef __USER_CONFIG_H__
#define __USER_CONFIG_H__

#include "et_types.h"
#include "et_fifo.h"

#define CMD_CONTROL   					0x70
#define CMD_CONTROL_ACK  				0x7F
#define CMD_QUERY						0x80
#define CMD_QUERY_ACK					0x8f

#define WIFI_LED_TIME_AIRKISS			300
#define WIFI_LED_TIME_SERVER_CONNECT  1000

#define TYPE_RGB_LIGHT_DEV  			0x10
#define TYPE_MY_SWITCH					0X20

#define START_LEN 						4   //sizeof(start_flag and length)
#define ACK_ERROR_LEN					5
#define ACK_TO_UART						START_LEN + ACK_ERROR_LEN
#define APPKEY_LEN 						48
#define UID_LEN  							APPKEY_LEN
#define SECRETKEY_LEN					UID_LEN
#define ET_LB_PORT_MAX  				9
#define ET_LB_ADDR_MAX 				128
#define MAX_MSG_LEN					128

#define ACK_SUCCESS         				0x00
#define ACK_BCC_ERROR     				0x01
#define ACK_DEV_FAILED					0x02
#define ACK_MODE_ERR					0x03
#define ACK_CMD_ILLIGAL  				0xFF
#define ACK_INVALID          				0xFE

#define MODE_INFO_FLASH_SECTOR		0xFC
#define APPKEY_UID_FASH_SECTOR		0xFE
#define SERVER_INFO_SECTOR				0xFF

#define SOFTWARE_VERSION  				"1.4.0.3-u34"

//#define USER_PRINT_DEBUG

#define RETURN_OK						(0)
#define RETURN_ERR						(-1)
#define AIRKISS_KEY_IO_NUM				(4)
#define AIRKISS_KEY_IO_PIN				(GPIO_Pin_4)
#define MAX_U16							(0xFFFF)

#define ARRAY_LEN(x)					(sizeof(x) / sizeof(x[0]))

typedef enum {
	WORK_MODE_RGB=2,
}WORK_MODE_T;

typedef struct id_info {
	et_uchar appkey[APPKEY_LEN];
	et_uchar uid[UID_LEN];
	et_uchar secretkey[SECRETKEY_LEN];
}id_info_t;

struct et_server_info {
	et_uchar lb_port[ET_LB_PORT_MAX];
	et_uchar lb_addr[ET_LB_ADDR_MAX];
};
typedef struct et_server_info et_server_info_t;

typedef struct key_gpio{
	et_uchar key_num;
	et_uint32 key_gpio_pin;
}key_gpio_t;

typedef struct msg_to_net {
	et_uchar buf[MAX_MSG_LEN];
	et_uint32 len;
}msg_to_net_t;

typedef struct msg_to_uart {
	et_uchar buf[MAX_MSG_LEN];
	et_uint32 len;
}msg_to_uart_t;

void airkiss_key_init(key_gpio_t*key);
et_int32 parse_msg_from_mqtt(et_uchar *msg_buf, et_int32 data_len);
SpiFlashOpResult get_uid(id_info_t *uid);
void wifi_led_spark(void *param);
void wifi_led_on();
void wifi_led_off();
void set_wifi_spark_timer(et_uint32 time);
void disarm_wifi_spark_timer();
void  et_user_main(void *pvParameters);
void print_hex(et_uchar *label, et_uchar *str, et_int32 len);
et_uchar check_sum(et_uchar *buf, et_int32 len);
#endif

