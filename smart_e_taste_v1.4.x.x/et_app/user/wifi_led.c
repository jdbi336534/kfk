#include "esp_common.h"
#include "driver/delay.h"
#include "driver/RGB_light.h"
#include "user_config.h"

os_timer_t wifi_led_spark_timer;

void wifi_led_on()
{
	RGB_light_set_color(0x00, 0xff, 0x00);
}

void wifi_led_off()
{
	RGB_light_set_color(0x00, 0x00, 0x00);
}

void wifi_led_spark(void *param)
{
	et_uint32 time = *((et_uint32 *)param);
	static et_uchar out_value = 0xff;

	if(time == WIFI_LED_TIME_AIRKISS)
	{
		RGB_light_set_color(out_value, 0x00, 0x00);
		out_value = ~out_value;			
	}
	else
	{
		RGB_light_set_color(0x00, out_value, 0x00);
		out_value = ~out_value;
	}
}

void set_wifi_spark_timer(et_uint32 time)
{
	static et_uint32 out_time = 0;

	out_time = time;
	os_timer_disarm(&wifi_led_spark_timer);
	os_timer_setfn(&wifi_led_spark_timer, (os_timer_func_t *)wifi_led_spark, (void *)&out_time);
	os_timer_arm(&wifi_led_spark_timer, out_time, 1);
}

void disarm_wifi_spark_timer()
{
	os_timer_disarm(&wifi_led_spark_timer);
}
