#include "espressif/esp_misc.h"
#include "espressif/c_types.h"
#include "et_types.h"
#include "driver/delay.h"


#define OS_DELAY_US os_delay_us
#define MAX_U16_VAL	(0xFFFF)

// —” ±Œ¢√Î		    								   
void delay_us(et_uint16 us)
{
	if (0 == us)
		return;

	OS_DELAY_US(us);
}

// —” ±∫¡√Î
void delay_ms(et_uint16 ms)
{
	et_uint16 i = 0, j = 0;
	et_uint32 sum = 0;
	
	if (0 == ms)
		return;
	
	sum = ms * 1000;
	i = sum / MAX_U16_VAL;
	j = sum % MAX_U16_VAL;
	
	while (0 < i--)
	{
		delay_us(MAX_U16_VAL);
	}
	delay_us(j);
}

// —” ±√Î
void delay_s(et_uint16 s)
{
	et_uint16 i = 0, j = 0;
	et_uint32 sum = 0;
	
	if (0 == s)
		return;
	
	sum = s * 1000;
	i = sum / MAX_U16_VAL;
	j = sum % MAX_U16_VAL;
	
	while (0 < i--)
	{
		delay_ms(MAX_U16_VAL);
	}
	delay_ms(j);
}
