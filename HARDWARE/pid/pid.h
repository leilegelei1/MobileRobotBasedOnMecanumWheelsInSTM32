#ifndef PID_H
#define PID_H
#include "stm32f10x.h"
#include "pwm_out.h"

extern float angel_now;

typedef struct pid_num//存放PID参数
{
	float kp;//p放大系数
	float ki;//i放大系数
	float kd;//d放大系数
	int i_const;//i限幅
	int pwm_total_const;//整体pwm得数限幅
}PID_NUM;

typedef struct wheel_pwm//每个电机有两个pwm来控制
{
	int go_pwm;
	int back_pwm;
}WHEEL_PWM;

typedef struct pid_pwm//用于存放所获得PWM
{
	WHEEL_PWM LeftForward;
	WHEEL_PWM RightForward;
	WHEEL_PWM LeftBehind;
	WHEEL_PWM RightBehind;
}PID_PWM;

typedef struct disire//用于存放目标速度
{
	int LeftForward;
	int RightForward;
	int LeftBehind;
	int RightBehind;
}DISIRE;

typedef struct pid_math
{
	float angle_now;
	float angle_past;
	int error_old;
	int error_now;
	int error_i;
	int error_d;
	int pwm_now;
	int pwm_old;
	int pwm_kp;
	int pwm_ki;
	int pwm_kd;
}PID_MATH;//用于存放PID计算过程中的一些数据

typedef struct pid_control//用于存放各个电机PID计算的数据
{
	PID_MATH LeftForward;
	PID_MATH RightForward;
	PID_MATH LeftBehind;
	PID_MATH RightBehind;
}PID_CONTROL;

typedef struct small_flag//用于存放期望速度是否过小，避免死区
{
	u8 LeftForward;
	u8 RightForward;
	u8 LeftBehind;
	u8 RightBehind; 
}SMALL_FLAG;


extern PID_PWM MyPwm;
extern DISIRE MyDisire;
extern PID_CONTROL MyControl;


void PID_CALL(DISIRE disire);

void PID_CAL(float disire);

void PID_INIT();


#endif

