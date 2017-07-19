#ifndef __USER_LIGHT_H__
#define __USER_LIGHT_H__

/*pwm.h: function and macro definition of PWM API , driver level */
/*RGB_light.h: user interface for light API, user level*/

#include "pwm.h"

struct light_saved_param {
    et_uint32  pwm_period;
    et_uint32  pwm_duty[PWM_CHANNEL];
};

#define LIGHT_RED				0
#define LIGHT_GREEN				1
#define LIGHT_BLUE				2
#define LIGHT_COLD_WHITE		3
#define LIGHT_WARM_WHITE		4

#define RGB_VAL_MIN		0
#define RGB_VAL_MAX		255

// ����/������������
//#define RGB_COMMON_VCC
#define RGB_COMMON_GND
// RGB value turn into duty
#define RGB_TO_DUTY(color, max_duty)	(color * max_duty / RGB_VAL_MAX)

void RGB_light_init(void);
et_uint32 RGB_light_get_duty(et_uint8 channel);
void RGB_light_set_duty(et_uint32 duty, et_uint8 channel);
et_uint32 RGB_light_get_period(void);
void RGB_light_set_period(et_uint32 period);
void RGB_light_set_color(et_uint8 r, et_uint8 g, et_uint8 b);

void RGB_light_start(void);

#endif

