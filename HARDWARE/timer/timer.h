/********************************************************************************
 * 文件名  ：timer.h
 * 描述    ：         
 * 实验平台：STM32F103ZET
 * PWM接口 		
 * 库版本  ：3.5.0
 * 作者    ：张磊
 * 时间    ：2015年8月20日10:42:12
**********************************************************************************/



#ifndef __TIMER_H
#define __TIMER_H
#include "stm32f10x.h"
void Timer6_Init(u16 arr,u16 psc);//刚刚错的原因是因为没有设置优先级
void Timer2_Init(u16 arr,u16 psc);
//void TIM4_INIT(void)
//float GET_NOWTIME(void);


#endif

