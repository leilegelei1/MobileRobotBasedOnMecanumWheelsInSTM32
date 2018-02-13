#include "pid.h"
#include "usart.h"
#include "delay.h"
#include "math.h"
int go_pwm,back_pwm;

float angel_now,angel_past;

int error_now,error_old,error_d,error_i;

u8 delay_flag=0;

int error_speed_d,error_speed_now,error_speed_old;

int angel_old=0;
int pwm_old=0;

const int pid_i_const=400;

int pwm_kp,pwm_kd,pwm_ki,pwm_pd;
int pwm_kp_speed,pwm_kd_speed,pwm_pd_speed;

float pid_kp=13.4;
float pid_ki=1.7;
float pid_kd=100.4;



//新版PWM控制

PID_NUM pid_para={13.4,2.4,100.4,400,1000};
PID_PWM MyPwm;
DISIRE MyDisire;
PID_CONTROL MyControl;
SMALL_FLAG MyFlag={0,0,0,0};

void PID_CAL(float disire)
{
	
//	if(angel_now==0)
//	{
//		if(disire>0)
//		{
//			TIM3_PWM_OUTPUT(200,0,0,0);
//		}
//		else
//			TIM3_PWM_OUTPUT(0,200,0,0);
//		
//		for(delay_flag=0;delay_flag<10;delay_flag++)
//			delay_ms(100);
//	}
	
	if(angel_now>110)
	{
		angel_now=angel_old;
		
	}
	if(angel_now<-110)
		angel_now=angel_old;
	
	if((disire>=-7)&&(disire<=7))
	{
		TIM3_PWM_OUTPUT(1000,1000,0,0);
		return;
	}
	
	//else if(angel_now-angel_old>40)
	//{
		//angel_now=angel_old;
		
	//}
	
	
	angel_old = angel_now;
	
	error_now=disire-angel_now;
	//printf("error %d\r\n",error_now);
	pwm_kp=pid_kp*error_now;
	
	
	error_d=error_now-error_old;
	
	pwm_kd=pid_kd*error_d;
	
	error_i+=error_now;
	if(error_i>pid_i_const)
		error_i=pid_i_const;
	pwm_ki=pid_ki*error_i;
	
	//printf("%d %d\r\n",error_now,error_old);
	
	error_old=error_now;
	
	pwm_pd=pwm_kp+pwm_kd+error_i;
	
	
	if((pwm_pd-pwm_old)>80)
		pwm_pd=pwm_old+80;
	if((pwm_pd-pwm_old)<-80)
		pwm_pd=pwm_old-80;
	
	pwm_old=pwm_pd;
	
	//printf("%d %d\r\n",pwm_kp,pwm_kd);
	go_pwm=1000+pwm_pd;
	back_pwm=1000-pwm_pd;
//	if(pwm_pd>=0)
//	{
//		go_pwm=2*pwm_pd;
//		back_pwm=0;
//	}
//	else
//	{
//		back_pwm=-2*pwm_pd;
//		go_pwm=0;
//	}
	
	if(go_pwm>2000)
		go_pwm=2000;
	else if(go_pwm<40)
		go_pwm=0;
	
	if(back_pwm>2000)
		back_pwm=2000;
	else if(back_pwm<40)
		back_pwm=0;
	
	TIM3_PWM_OUTPUT(go_pwm,back_pwm,0,0);
	
	//printf("%f  %d  %d %d\r\n",angel_now,error_now,go_pwm,back_pwm);

	
}

void PID_INIT()
{
	MyControl.LeftForward.angle_now=0;
	MyControl.LeftForward.angle_past=0;
	MyControl.LeftForward.error_d=0;
	MyControl.LeftForward.error_i=0;
	MyControl.LeftForward.error_now=0;
	MyControl.LeftForward.error_old=0;
	MyControl.LeftForward.pwm_kd=0;
	MyControl.LeftForward.pwm_ki=0;
	MyControl.LeftForward.pwm_kp=0;
	MyControl.LeftForward.pwm_now=0;
	MyControl.LeftForward.pwm_old=0;
	
	MyControl.RightBehind.angle_now=0;
	MyControl.RightBehind.angle_past=0;
	MyControl.RightBehind.error_d=0;
	MyControl.RightBehind.error_i=0;
	MyControl.RightBehind.error_now=0;
	MyControl.RightBehind.error_old=0;
	MyControl.RightBehind.pwm_kd=0;
	MyControl.RightBehind.pwm_ki=0;
	MyControl.RightBehind.pwm_kp=0;
	MyControl.RightBehind.pwm_now=0;
	MyControl.RightBehind.pwm_old=0;
	
	MyControl.RightForward.angle_now=0;
	MyControl.RightForward.angle_past=0;
	MyControl.RightForward.error_d=0;
	MyControl.RightForward.error_i=0;
	MyControl.RightForward.error_now=0;
	MyControl.RightForward.error_old=0;
	MyControl.RightForward.pwm_kd=0;
	MyControl.RightForward.pwm_ki=0;
	MyControl.RightForward.pwm_kp=0;
	MyControl.RightForward.pwm_now=0;
	MyControl.RightForward.pwm_old=0;
	
	MyControl.LeftBehind.angle_now=0;
	MyControl.LeftBehind.angle_past=0;
	MyControl.LeftBehind.error_d=0;
	MyControl.LeftBehind.error_i=0;
	MyControl.LeftBehind.error_now=0;
	MyControl.LeftBehind.error_old=0;
	MyControl.LeftBehind.pwm_kd=0;
	MyControl.LeftBehind.pwm_ki=0;
	MyControl.LeftBehind.pwm_kp=0;
	MyControl.LeftBehind.pwm_now=0;
	MyControl.LeftBehind.pwm_old=0;
	
}

void PID_CALL(DISIRE disire)
{	
	//1.右前 2.左前 3.左后 4.右后
	
	//先将标志位清零
	{
		MyFlag.LeftBehind=0;
		MyFlag.LeftForward=0;
		MyFlag.RightBehind=0;
		MyFlag.RightForward=0;
	}
	
	//先判别数据是否有错误
	{
		if(MyControl.LeftForward.angle_now >95)
			MyControl.LeftForward.angle_now=MyControl.LeftForward.angle_past;
	  if(MyControl.LeftForward.angle_now<-95)
			MyControl.LeftForward.angle_now=MyControl.LeftForward.angle_past;
		
		if(MyControl.LeftBehind.angle_now >95)
			MyControl.LeftBehind.angle_now=MyControl.LeftBehind.angle_past;
	  if(MyControl.LeftBehind.angle_now<-95)
			MyControl.LeftBehind.angle_now=MyControl.LeftBehind.angle_past;
		
		if(MyControl.RightForward.angle_now >95)
			MyControl.RightForward.angle_now=MyControl.RightForward.angle_past;
	  if(MyControl.RightForward.angle_now<-95)
			MyControl.RightForward.angle_now=MyControl.RightForward.angle_past;
		
		if(MyControl.RightBehind.angle_now >95)
			MyControl.RightBehind.angle_now=MyControl.RightBehind.angle_past;
	  if(MyControl.RightBehind.angle_now<-95)
			MyControl.RightBehind.angle_now=MyControl.RightBehind.angle_past;
	}
	
	{
		if((MyDisire.LeftForward>=-7)&&(MyDisire.LeftForward<=7))
		{
			MyPwm.LeftForward.go_pwm=1000;
			MyPwm.LeftForward.back_pwm=1000;
			MyFlag.LeftForward=1;
		}
		
		if((MyDisire.RightForward>=-7)&&(MyDisire.RightForward<=7))
		{
			MyPwm.RightForward.go_pwm=1000;
			MyPwm.RightForward.back_pwm=1000;
			MyFlag.RightForward=1;
		}
		
		if((MyDisire.LeftBehind >=-7)&&(MyDisire.LeftBehind<=7))
		{
			MyPwm.LeftBehind.go_pwm=1000;
			MyPwm.LeftBehind.back_pwm=1000;
			MyFlag.LeftBehind=1;
		}
		
		if((MyDisire.RightBehind >=-7)&&(MyDisire.RightBehind<=7))
		{
			MyPwm.RightBehind.go_pwm=1000;
			MyPwm.RightBehind.back_pwm=1000;
			MyFlag.RightBehind=1;
		}
		
	}
	
	{
		MyControl.LeftForward.angle_past=MyControl.LeftForward.angle_now;
		MyControl.RightForward.angle_past=MyControl.RightForward.angle_now;
		MyControl.LeftBehind.angle_past=MyControl.LeftBehind.angle_now;
		MyControl.RightBehind.angle_past=MyControl.RightBehind.angle_now;
	}
	
	//printf("%f %f %f %f\r\n",MyControl.RightForward.angle_now,MyControl.LeftForward.angle_now,
							//MyControl.LeftBehind.angle_now,MyControl.RightBehind.angle_now);
	
	//1号电机控速
	{	
		if(!MyFlag.RightForward)
		{
	
			MyControl.RightForward.error_now=MyDisire.RightForward - MyControl.RightForward.angle_now;
			//printf("error %d\r\n",error_now);
			MyControl.RightForward.pwm_kp= pid_para.kp * MyControl.RightForward.error_now;
			//pwm_kp=pid_kp*error_now;
			MyControl.RightForward.error_d= MyControl.RightForward.error_now -MyControl.RightForward.error_old ;
			//error_d=error_now-error_old;
			MyControl.RightForward.pwm_kd= pid_para.kd * MyControl.RightForward.error_d;
			//pwm_kd=pid_kd*error_d;
			MyControl.RightForward.error_i += MyControl.RightForward.error_now;
			//error_i+=error_now;
			if(MyControl.RightForward.error_i>pid_para.i_const)
				MyControl.RightForward.error_i=pid_para.i_const;
			if(MyControl.RightForward.error_i<(-1*pid_para.i_const))
				MyControl.RightForward.error_i=(-1*pid_para.i_const);
			//if(error_i>pid_i_const)
			//	error_i=pid_i_const;
			MyControl.RightForward.pwm_ki= pid_para.ki * MyControl.RightForward.error_i;
			//pwm_ki=pid_ki*error_i;
			//printf("%d %d\r\n",error_now,error_old);
			MyControl.RightForward.error_old=MyControl.RightForward.error_now;
			//error_old=error_now;
			MyControl.RightForward.pwm_now=MyControl.RightForward.pwm_kp+MyControl.RightForward.pwm_ki+MyControl.RightForward.pwm_kd;
			//pwm_pd=pwm_kp+pwm_kd+error_i;
			if((MyControl.RightForward.pwm_now-MyControl.RightForward.pwm_old)>80)
				MyControl.RightForward.pwm_now=MyControl.RightForward.pwm_old+80;
			if((MyControl.RightForward.pwm_now-MyControl.RightForward.pwm_old)<-80)
				MyControl.RightForward.pwm_now=MyControl.RightForward.pwm_old-80;
			
			if(MyControl.RightForward.pwm_now>pid_para.pwm_total_const)
				MyControl.RightForward.pwm_now=pid_para.pwm_total_const;
			else if(MyControl.RightForward.pwm_now<(-1*pid_para.pwm_total_const))
				MyControl.RightForward.pwm_now=-1*pid_para.pwm_total_const;
			
//			if((pwm_pd-pwm_old)>80)
//				pwm_pd=pwm_old+80;
//			if((pwm_pd-pwm_old)<-80)
//				pwm_pd=pwm_old-80;
			MyControl.RightForward.pwm_old=MyControl.RightForward.pwm_now;
			//pwm_old=pwm_pd;
			//printf("%d %d\r\n",pwm_kp,pwm_kd);
			
			
			MyPwm.RightForward.go_pwm = 1000+MyControl.RightForward.pwm_now;
			MyPwm.RightForward.back_pwm = 1000-MyControl.RightForward.pwm_now;
		//	go_pwm=1000+pwm_pd;
		//	back_pwm=1000-pwm_pd;
			if(MyPwm.RightForward.go_pwm>2000)
				MyPwm.RightForward.go_pwm=2000;
			else if(MyPwm.RightForward.go_pwm<40)
				MyPwm.RightForward.go_pwm=40;
			
			if(MyPwm.RightForward.back_pwm>2000)
				MyPwm.RightForward.back_pwm=2000;
			else if(MyPwm.RightForward.back_pwm<40)
				MyPwm.RightForward.back_pwm=40;
				
			//printf("%f %d %d %d\r\n",MyControl.RightForward.angle_now,MyControl.RightForward.pwm_kp,MyControl.RightForward.pwm_ki,MyControl.RightForward.pwm_kd);
//			if(go_pwm>2000)
//				go_pwm=2000;
//			else if(go_pwm<40)
//				go_pwm=0;
//	
//			if(back_pwm>2000)
//				back_pwm=2000;
//			else if(back_pwm<40)
//				back_pwm=0;
		}
	}
	
	{//二号电机控速
		if(!MyFlag.LeftForward)
		{
	
			MyControl.LeftForward.error_now=MyDisire.LeftForward - MyControl.LeftForward.angle_now;
			//printf("error %d\r\n",error_now);
			MyControl.LeftForward.pwm_kp= pid_para.kp * MyControl.LeftForward.error_now;
			//pwm_kp=pid_kp*error_now;
			MyControl.LeftForward.error_d= MyControl.LeftForward.error_now -MyControl.LeftForward.error_old ;
			//error_d=error_now-error_old;
			MyControl.LeftForward.pwm_kd= pid_para.kd * MyControl.LeftForward.error_d;
			//pwm_kd=pid_kd*error_d;
			MyControl.LeftForward.error_i += MyControl.LeftForward.error_now;
			//error_i+=error_now;
			if(MyControl.LeftForward.error_i>pid_para.i_const)
				MyControl.LeftForward.error_i=pid_para.i_const;
			if(MyControl.LeftForward.error_i<(-1*pid_para.i_const))
				MyControl.LeftForward.error_i=(-1*pid_para.i_const);
			//if(error_i>pid_i_const)
			//	error_i=pid_i_const;
			MyControl.LeftForward.pwm_ki= pid_para.ki * MyControl.LeftForward.error_i;
			//pwm_ki=pid_ki*error_i;
			//printf("%d %d\r\n",error_now,error_old);
			MyControl.LeftForward.error_old=MyControl.LeftForward.error_now;
			//error_old=error_now;
			MyControl.LeftForward.pwm_now=MyControl.LeftForward.pwm_kp+MyControl.LeftForward.pwm_ki+MyControl.LeftForward.pwm_kd;
			//pwm_pd=pwm_kp+pwm_kd+error_i;
			if((MyControl.LeftForward.pwm_now-MyControl.LeftForward.pwm_old)>80)
				MyControl.LeftForward.pwm_now=MyControl.LeftForward.pwm_old+80;
			if((MyControl.LeftForward.pwm_now-MyControl.LeftForward.pwm_old)<-80)
				MyControl.LeftForward.pwm_now=MyControl.LeftForward.pwm_old-80;
			
			if(MyControl.LeftForward.pwm_now>pid_para.pwm_total_const)
				MyControl.LeftForward.pwm_now=pid_para.pwm_total_const;
			else if(MyControl.LeftForward.pwm_now<(-1*pid_para.pwm_total_const))
				MyControl.LeftForward.pwm_now=-1*pid_para.pwm_total_const;
			
//			if((pwm_pd-pwm_old)>80)
//				pwm_pd=pwm_old+80;
//			if((pwm_pd-pwm_old)<-80)
//				pwm_pd=pwm_old-80;
			MyControl.LeftForward.pwm_old=MyControl.LeftForward.pwm_now;
			//pwm_old=pwm_pd;
			//printf("%d %d\r\n",pwm_kp,pwm_kd);
			
			MyPwm.LeftForward.go_pwm = 1000+MyControl.LeftForward.pwm_now;
			MyPwm.LeftForward.back_pwm = 1000-MyControl.LeftForward.pwm_now;
		//	go_pwm=1000+pwm_pd;
		//	back_pwm=1000-pwm_pd;
			if(MyPwm.LeftForward.go_pwm>2000)
				MyPwm.LeftForward.go_pwm=2000;
			else if(MyPwm.LeftForward.go_pwm<40)
				MyPwm.LeftForward.go_pwm=40;
			
			if(MyPwm.LeftForward.back_pwm>2000)
				MyPwm.LeftForward.back_pwm=2000;
			else if(MyPwm.LeftForward.back_pwm<40)
				MyPwm.LeftForward.back_pwm=40;
				
//			if(go_pwm>2000)
//				go_pwm=2000;
//			else if(go_pwm<40)
//				go_pwm=0;
//	
//			if(back_pwm>2000)
//				back_pwm=2000;
//			else if(back_pwm<40)
//				back_pwm=0;
		}
	}
	
	{//三号电机控速
		if(!MyFlag.LeftBehind)
		{
	
			MyControl.LeftBehind.error_now=MyDisire.LeftBehind - MyControl.LeftBehind.angle_now;
			//printf("error %d\r\n",error_now);
			MyControl.LeftBehind.pwm_kp= pid_para.kp * MyControl.LeftBehind.error_now;
			//pwm_kp=pid_kp*error_now;
			MyControl.LeftBehind.error_d= MyControl.LeftBehind.error_now -MyControl.LeftBehind.error_old ;
			//error_d=error_now-error_old;
			MyControl.LeftBehind.pwm_kd= pid_para.kd * MyControl.LeftBehind.error_d;
			//pwm_kd=pid_kd*error_d;
			MyControl.LeftBehind.error_i += MyControl.LeftBehind.error_now;
			//error_i+=error_now;
			if(MyControl.LeftBehind.error_i>pid_para.i_const)
				MyControl.LeftBehind.error_i=pid_para.i_const;
			if(MyControl.LeftBehind.error_i<(-1*pid_para.i_const))
				MyControl.LeftBehind.error_i=(-1*pid_para.i_const);
			//if(error_i>pid_i_const)
			//	error_i=pid_i_const;
			MyControl.LeftBehind.pwm_ki= pid_para.ki * MyControl.LeftBehind.error_i;
			//pwm_ki=pid_ki*error_i;
			//printf("%d %d\r\n",error_now,error_old);
			MyControl.LeftBehind.error_old=MyControl.LeftBehind.error_now;
			//error_old=error_now;
			MyControl.LeftBehind.pwm_now=MyControl.LeftBehind.pwm_kp+MyControl.LeftBehind.pwm_ki+MyControl.LeftBehind.pwm_kd;
			//pwm_pd=pwm_kp+pwm_kd+error_i;
			if((MyControl.LeftBehind.pwm_now-MyControl.LeftBehind.pwm_old)>80)
				MyControl.LeftBehind.pwm_now=MyControl.LeftBehind.pwm_old+80;
			if((MyControl.LeftBehind.pwm_now-MyControl.LeftBehind.pwm_old)<-80)
				MyControl.LeftBehind.pwm_now=MyControl.LeftBehind.pwm_old-80;
			
			if(MyControl.LeftBehind.pwm_now>pid_para.pwm_total_const)
				MyControl.LeftBehind.pwm_now=pid_para.pwm_total_const;
			else if(MyControl.LeftBehind.pwm_now<(-1*pid_para.pwm_total_const))
				MyControl.LeftBehind.pwm_now=-1*pid_para.pwm_total_const;
			
//			if((pwm_pd-pwm_old)>80)
//				pwm_pd=pwm_old+80;
//			if((pwm_pd-pwm_old)<-80)
//				pwm_pd=pwm_old-80;
			MyControl.LeftBehind.pwm_old=MyControl.LeftBehind.pwm_now;
			//pwm_old=pwm_pd;
			//printf("%d %d\r\n",pwm_kp,pwm_kd);
			
			MyPwm.LeftBehind.go_pwm = 1000+MyControl.LeftBehind.pwm_now;
			MyPwm.LeftBehind.back_pwm = 1000-MyControl.LeftBehind.pwm_now;
		//	go_pwm=1000+pwm_pd;
		//	back_pwm=1000-pwm_pd;
			if(MyPwm.LeftBehind.go_pwm>2000)
				MyPwm.LeftBehind.go_pwm=2000;
			else if(MyPwm.LeftBehind.go_pwm<40)
				MyPwm.LeftBehind.go_pwm=40;
			
			if(MyPwm.LeftBehind.back_pwm>2000)
				MyPwm.LeftBehind.back_pwm=2000;
			else if(MyPwm.LeftBehind.back_pwm<40)
				MyPwm.LeftBehind.back_pwm=40;
				
//			if(go_pwm>2000)
//				go_pwm=2000;
//			else if(go_pwm<40)
//				go_pwm=0;
//	
//			if(back_pwm>2000)
//				back_pwm=2000;
//			else if(back_pwm<40)
//				back_pwm=0;
		}
	}
	
	{//四号电机控速
		if(!MyFlag.RightBehind)
		{
	
			MyControl.RightBehind.error_now=MyDisire.RightBehind - MyControl.RightBehind.angle_now;
			//printf("error %d\r\n",error_now);
			MyControl.RightBehind.pwm_kp= pid_para.kp * MyControl.RightBehind.error_now;
			//pwm_kp=pid_kp*error_now;
			MyControl.RightBehind.error_d= MyControl.RightBehind.error_now -MyControl.RightBehind.error_old ;
			//error_d=error_now-error_old;
			MyControl.RightBehind.pwm_kd= pid_para.kd * MyControl.RightBehind.error_d;
			//pwm_kd=pid_kd*error_d;
			MyControl.RightBehind.error_i += MyControl.RightBehind.error_now;
			//error_i+=error_now;
			if(MyControl.RightBehind.error_i>pid_para.i_const)
				MyControl.RightBehind.error_i=pid_para.i_const;
			if(MyControl.RightBehind.error_i<(-1*pid_para.i_const))
				MyControl.RightBehind.error_i=(-1*pid_para.i_const);
			//if(error_i>pid_i_const)
			//	error_i=pid_i_const;
			MyControl.RightBehind.pwm_ki= pid_para.ki * MyControl.RightBehind.error_i;
			//pwm_ki=pid_ki*error_i;
			//printf("%d %d\r\n",error_now,error_old);
			MyControl.RightBehind.error_old=MyControl.RightBehind.error_now;
			//error_old=error_now;
			MyControl.RightBehind.pwm_now=MyControl.RightBehind.pwm_kp+MyControl.RightBehind.pwm_ki+MyControl.RightBehind.pwm_kd;
			//pwm_pd=pwm_kp+pwm_kd+error_i;
			if((MyControl.RightBehind.pwm_now-MyControl.RightBehind.pwm_old)>80)
				MyControl.RightBehind.pwm_now=MyControl.RightBehind.pwm_old+80;
			if((MyControl.RightBehind.pwm_now-MyControl.RightBehind.pwm_old)<-80)
				MyControl.RightBehind.pwm_now=MyControl.RightBehind.pwm_old-80;
			
			if(MyControl.RightBehind.pwm_now>pid_para.pwm_total_const)
				MyControl.RightBehind.pwm_now=pid_para.pwm_total_const;
			else if(MyControl.RightBehind.pwm_now<(-1*pid_para.pwm_total_const))
				MyControl.RightBehind.pwm_now=-1*pid_para.pwm_total_const;
			
//			if((pwm_pd-pwm_old)>80)
//				pwm_pd=pwm_old+80;
//			if((pwm_pd-pwm_old)<-80)
//				pwm_pd=pwm_old-80;
			MyControl.RightBehind.pwm_old=MyControl.RightBehind.pwm_now;
			//pwm_old=pwm_pd;
			//printf("%d %d\r\n",pwm_kp,pwm_kd);
			
			MyPwm.RightBehind.go_pwm = 1000+MyControl.RightBehind.pwm_now;
			MyPwm.RightBehind.back_pwm = 1000-MyControl.RightBehind.pwm_now;
		//	go_pwm=1000+pwm_pd;
		//	back_pwm=1000-pwm_pd;
			if(MyPwm.RightBehind.go_pwm>2000)
				MyPwm.RightBehind.go_pwm=2000;
			else if(MyPwm.RightBehind.go_pwm<40)
				MyPwm.RightBehind.go_pwm=40;
			
			if(MyPwm.RightBehind.back_pwm>2000)
				MyPwm.RightBehind.back_pwm=2000;
			else if(MyPwm.RightBehind.back_pwm<40)
				MyPwm.RightBehind.back_pwm=40;
				
//			if(go_pwm>2000)
//				go_pwm=2000;
//			else if(go_pwm<40)
//				go_pwm=0;
//	
//			if(back_pwm>2000)
//				back_pwm=2000;
//			else if(back_pwm<40)
//				back_pwm=0;
		}
	}
	TIM_PWM_SET(MyPwm.RightForward.go_pwm,MyPwm.RightForward.back_pwm,MyPwm.LeftForward.go_pwm,MyPwm.LeftForward.back_pwm,
							MyPwm.LeftBehind.go_pwm,MyPwm.LeftBehind.back_pwm,MyPwm.RightBehind.go_pwm,MyPwm.RightBehind.back_pwm);//设置速度
}


