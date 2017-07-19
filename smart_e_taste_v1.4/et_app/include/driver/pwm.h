#ifndef __PWM_H__
#define __PWM_H__

/*pwm.h: function and macro definition of PWM API , driver level */
/*RGB_light.h: user interface for light API, user level*/


/*Definition of GPIO PIN params, for GPIO initialization*/
#define PWM_0_OUT_IO_MUX PERIPHS_IO_MUX_MTDI_U
#define PWM_0_OUT_IO_NUM 12
#define PWM_0_OUT_IO_FUNC  FUNC_GPIO12
#define PWM_1_OUT_IO_MUX PERIPHS_IO_MUX_MTDO_U
#define PWM_1_OUT_IO_NUM 15
#define PWM_1_OUT_IO_FUNC  FUNC_GPIO15
#define PWM_2_OUT_IO_MUX PERIPHS_IO_MUX_MTCK_U
#define PWM_2_OUT_IO_NUM 13
#define PWM_2_OUT_IO_FUNC  FUNC_GPIO13
#define PWM_3_OUT_IO_MUX PERIPHS_IO_MUX_MTMS_U
#define PWM_3_OUT_IO_NUM 14
#define PWM_3_OUT_IO_FUNC  FUNC_GPIO14
#define PWM_4_OUT_IO_MUX PERIPHS_IO_MUX_GPIO5_U
#define PWM_4_OUT_IO_NUM 5
#define PWM_4_OUT_IO_FUNC  FUNC_GPIO5


/* define the max\min period for PWM, unit is us*/
#define PWM_PERIOD_MAX	10000
#define PWM_PERIOD_MIN	1000

/* define the max\min duty for PWM */
#define PWM_DUTY_MAX(period)	(period * 1000 / 45)	
#define PWM_DUTY_MIN			0

/*Define the channel number of PWM*/
/*In this demo, we can set 3 for 3 PWM channels: RED, GREEN, BLUE*/
/*Or , we can choose 5 channels : RED,GREEN,BLUE,COLD-WHITE,WARM-WHITE*/
#define PWM_CHANNEL 3  /*5:5channel ; 3:3channel*/

/*SUPPORT UP TO 8 PWM CHANNEL*/
#define PWM_CHANNEL_NUM_MAX 8 

struct pwm_param {
    uint32 period;
    uint32 freq;
    uint32 duty[PWM_CHANNEL_NUM_MAX];  //PWM_CHANNEL<=8
};


/* pwm_init should be called only once, for now  */
void pwm_init(uint32 period, uint32 *duty,uint32 pwm_channel_num,uint32 (*pin_info_list)[3]);
void pwm_start(void);

void pwm_set_duty(uint32 duty, uint8 channel);
uint32 pwm_get_duty(uint8 channel);
void pwm_set_period(uint32 period);
uint32 pwm_get_period(void);

uint32 get_pwm_version(void);
void set_pwm_debug_en(uint8 print_en);


#endif
