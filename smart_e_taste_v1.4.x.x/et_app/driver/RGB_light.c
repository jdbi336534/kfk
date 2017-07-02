/******************************************************************************
 * Copyright 2013-2014 Espressif Systems (Wuxi)
 *
 * FileName: RGB_light.c
 *
 * Description: light demo's function realization
 *
 * Modification history:
 *     2014/5/1, v1.0 create this file.
*******************************************************************************/
#include "esp_common.h"
#include "driver/RGB_light.h"
#include "driver/pwm.h"
#include "user_config.h"

struct light_saved_param light_param;
static BOOL update_flag = FALSE;
/******************************************************************************
 * FunctionName : RGB_light_get_duty
 * Description  : get duty of each channel
 * Parameters   : et_uint8 channel : LIGHT_RED/LIGHT_GREEN/LIGHT_BLUE
 * Returns      : NONE
*******************************************************************************/
et_uint32 ICACHE_FLASH_ATTR
RGB_light_get_duty(et_uint8 channel)
{
    return light_param.pwm_duty[channel];
}

/******************************************************************************
 * FunctionName : RGB_light_set_duty
 * Description  : set each channel's duty params
 * Parameters   : et_uint8 duty    : 0 ~ PWM_DEPTH
 *                et_uint8 channel : LIGHT_RED/LIGHT_GREEN/LIGHT_BLUE
 * Returns      : NONE
*******************************************************************************/
void ICACHE_FLASH_ATTR
RGB_light_set_duty(et_uint32 duty, et_uint8 channel)
{	
    if (duty != light_param.pwm_duty[channel]) {
        pwm_set_duty(duty, channel);
		update_flag = TRUE;
        light_param.pwm_duty[channel] = pwm_get_duty(channel);
    }
}

/******************************************************************************
 * FunctionName : RGB_light_get_period
 * Description  : get pwm period
 * Parameters   : NONE
 * Returns      : et_uint32 : pwm period
*******************************************************************************/
et_uint32 ICACHE_FLASH_ATTR
RGB_light_get_period(void)
{
    return light_param.pwm_period;
}

/******************************************************************************
 * FunctionName : RGB_light_set_period
 * Description  : set pwm period
 * Parameters   : uint16 freq : 100hz typically
 * Returns      : NONE
*******************************************************************************/
void ICACHE_FLASH_ATTR
RGB_light_set_period(et_uint32 period)
{
    if (period != light_param.pwm_period) {
        pwm_set_period(period);
		update_flag = TRUE;
        light_param.pwm_period = pwm_get_period();
    }
}

/******************************************************************************
 * FunctionName : RGB_light_start
 * Description  : update pwm params
 * Parameters   : uint16 freq : 100hz typically
 * Returns      : NONE
*******************************************************************************/
void ICACHE_FLASH_ATTR
RGB_light_start(void)
{
	if (TRUE == update_flag)
	{
		update_flag = FALSE;
		pwm_start();
	}
}

/******************************************************************************
 * FunctionName : RGB_light_set_color
 * Description  : set pwm period
 * Parameters   : uint16 freq : 100hz typically
 * Returns      : NONE
*******************************************************************************/
void ICACHE_FLASH_ATTR
RGB_light_set_color(et_uint8 r, et_uint8 g, et_uint8 b)
{
//	et_uint32 period = RGB_light_get_period();
//	et_uint32 max_duty = PWM_DUTY_MAX(period);
	et_uint32 max_duty_r = 1024;
	et_uint32 max_duty_g = 512;
	et_uint32 max_duty_b = 1024;


#ifdef USER_PRINT_DEBUG
	printf("set color: R(%02x) G(%02x) B(%02x)\r\n", r, g, b);
#endif

	// red
	if (0xfd == r)
	{
		r = 0xff;
		g = 0x00;
		b = 0x00;
	}
	else if (0xfc == r)
	{
		r = 0xff;
		g = 0x7f;
		b = 0x00;
	}
	
	// green
	
	// blue
	
#ifdef RGB_COMMON_GND
	RGB_light_set_duty(RGB_TO_DUTY(r, max_duty_r), LIGHT_RED);
	RGB_light_set_duty(RGB_TO_DUTY(g, max_duty_g), LIGHT_GREEN);
	RGB_light_set_duty(RGB_TO_DUTY(b, max_duty_b), LIGHT_BLUE);
#else
	RGB_light_set_duty(RGB_TO_DUTY((et_uint8)~r, max_duty_r), LIGHT_RED);
	RGB_light_set_duty(RGB_TO_DUTY((et_uint8)~g, max_duty_g), LIGHT_GREEN);
	RGB_light_set_duty(RGB_TO_DUTY((et_uint8)~b, max_duty_b), LIGHT_BLUE);
#endif
	RGB_light_start();
}

/******************************************************************************
 * FunctionName : RGB_light_init
 * Description  : light demo init, mainy init pwm
 * Parameters   : none
 * Returns      : none
*******************************************************************************/
void ICACHE_FLASH_ATTR
RGB_light_init(void)
{
	memset((void*)&light_param, 0, sizeof(light_param));
	if (light_param.pwm_period > PWM_PERIOD_MAX || light_param.pwm_period < PWM_PERIOD_MIN)
	{
		light_param.pwm_period = PWM_PERIOD_MIN;
    }

	et_uint32 pin_info[PWM_CHANNEL][3] = 
								{
									{PWM_1_OUT_IO_MUX,PWM_1_OUT_IO_FUNC,PWM_1_OUT_IO_NUM},		
									{PWM_0_OUT_IO_MUX,PWM_0_OUT_IO_FUNC,PWM_0_OUT_IO_NUM},		 
									{PWM_2_OUT_IO_MUX,PWM_2_OUT_IO_FUNC,PWM_2_OUT_IO_NUM}
								};
	
	et_uint32 pwm_duty_init[PWM_CHANNEL] = {0};
	
    /*PIN FUNCTION INIT FOR PWM OUTPUT*/
    pwm_init(light_param.pwm_period, pwm_duty_init, PWM_CHANNEL, pin_info);

#ifdef USER_PRINT_DEBUG    
    printf("LIGHT PARAM: R: %d \r\n",light_param.pwm_duty[LIGHT_RED]);
    printf("LIGHT PARAM: G: %d \r\n",light_param.pwm_duty[LIGHT_GREEN]);
    printf("LIGHT PARAM: B: %d \r\n",light_param.pwm_duty[LIGHT_BLUE]);

	if (PWM_CHANNEL > LIGHT_WARM_WHITE) 
	{
		printf("LIGHT PARAM: CW: %d \r\n",light_param.pwm_duty[LIGHT_COLD_WHITE]);
		printf("LIGHT PARAM: WW: %d \r\n",light_param.pwm_duty[LIGHT_WARM_WHITE]);
	}
    printf("LIGHT PARAM: P: %d \r\n",light_param.pwm_period);
#endif

}


