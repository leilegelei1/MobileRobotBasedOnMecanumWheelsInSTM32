#ifndef PID_H
#define PID_H
#include "stm32f10x.h"
#include "pwm_out.h"

extern float angel_now;

typedef struct pid_num//���PID����
{
	float kp;//p�Ŵ�ϵ��
	float ki;//i�Ŵ�ϵ��
	float kd;//d�Ŵ�ϵ��
	int i_const;//i�޷�
	int pwm_total_const;//����pwm�����޷�
}PID_NUM;

typedef struct wheel_pwm//ÿ�����������pwm������
{
	int go_pwm;
	int back_pwm;
}WHEEL_PWM;

typedef struct pid_pwm//���ڴ�������PWM
{
	WHEEL_PWM LeftForward;
	WHEEL_PWM RightForward;
	WHEEL_PWM LeftBehind;
	WHEEL_PWM RightBehind;
}PID_PWM;

typedef struct disire//���ڴ��Ŀ���ٶ�
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
}PID_MATH;//���ڴ��PID��������е�һЩ����

typedef struct pid_control//���ڴ�Ÿ������PID���������
{
	PID_MATH LeftForward;
	PID_MATH RightForward;
	PID_MATH LeftBehind;
	PID_MATH RightBehind;
}PID_CONTROL;

typedef struct small_flag//���ڴ�������ٶ��Ƿ��С����������
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

