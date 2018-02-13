#ifndef __PWNIN_H
#define	__PWNIN_H

#include "stm32f10x.h"

extern int pwmout1,pwmout2,pwmout3,pwmout4;

extern int pwmout11,pwmout12,pwmout13,pwmout14;

extern unsigned char soundflag;

extern int speed1;

extern u8 SPEED_ROLL_FLAG;

extern u8 whatup;

typedef struct trunflag
{
	u8 RF_TURN;
	u8 LF_TURN;
	u8 LB_TURN;
	u8 RB_TURN;
}TURNFLAG;

extern TURNFLAG MyTurnflag;



extern unsigned char Control_Mode;
//void TIM5_Cap_Init(u16 arr,u16 psc);
void pwm_in_init(void);
void tim1_pwm_in_init(void);
void PIN2_Init();
void DeHight(void);


#endif/* __PWNIN */
