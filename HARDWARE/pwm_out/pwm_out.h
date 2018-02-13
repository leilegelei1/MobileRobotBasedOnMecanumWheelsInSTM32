//编译环境 MDK5
//库函数版本 3.5.0
//作者：张磊
//日期：2015年8月19日23:47:14


#ifndef _PWM_OUT
#define _PWM_OUT

#include "stm32f10x.h"

void TIM3_PWM_Init(u16 arr,u16 psc);
void TIM3_PWM_OUTPUT(u16 DR1,u16 DR2,u16 DR3,u16 DR4);

void TIM2_PWM_Init(u16 arr,u16 psc);
void TIM2_PWM_OUTPUT(u16 DR1,u16 DR2,u16 DR3,u16 DR4);

void TIM_PWM_SET(u16 DR1,u16 DR2,u16 DR3,u16 DR4,u16 DR5,u16 DR6,u16 DR7,u16 DR8);

void TIM5_PWM_Init(void);
void TIM5_PWM_OUTPUT(u16 DR3,u16 DR4);
void sound_pwm(u16 DR1);
#endif

