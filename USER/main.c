#include "stm32f10x.h"
#include "adc.h"
#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "timer.h"
#include "pwm_out.h"
#include "pwm_in.h"
#include "pid.h"
#include "led.h"
#include "24cxx.h"
#include "myiic.h"
#include "kinematics.h"
#include "data.h"
 void Delay(u32 count)
 {
   u32 i=0;
	 
   for(;i<count;i++);
 }
 
 extern u8 USART_RX_BUF[USART_REC_LEN];

 int DisireAngle;

 int controlflag=0;
 
  u8 Angle_flag=0;
 
 u8 tim2flag;
 
 u8 uartrecived[7];
 
 u8 iic_buf[6];
 
 u8 charbuf=0;
 
 
 float justtest;
 
 u8 flagchange=0;
 
 u8 sendflag=0;
 
 char i;
 
 char test;
 
 extern float pid_kp,pid_kd,pid_ki;
 
 int main(void)
 {	
	 u8  testbuf[9]={0xfd,0xfd,0xfd,0xfd,0xfd,0xfd,0xfd,0xfd,0xfd};
		u8  testnum=0;
	 
	u16 adc15,adc14;
	float temp,temp1;
	extern float angel_now;
	float disire_angel;
	delay_init();	    	 //ÑÓÊ±º¯Êý³õÊ¼»¯	
	//Timer2_Init(20,7199);//TIME2³õÊ¼»¯
	 controlflag=0;
	 Timer6_Init(60,7199);
	 tim1_pwm_in_init();
	 pwm_in_init();
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//ÉèÖÃÖÐ¶ÏÓÅÏÈ¼¶·Ö×éÎª×é2£º2Î»ÇÀÕ¼ÓÅÏÈ¼¶£¬2Î»ÏìÓ¦ÓÅÏÈ¼¶
	 uart_init(115200);	 	//´®¿Ú³õÊ¼»¯Îª115200
	 MatrixInit();
	 //LED_Init();
	 
	 TIM3_PWM_Init(2000-1,72-1);
	 TIM2_PWM_Init(2000-1,72-1);//PWMÇý¶¯³õÊ¼»¯
	 
	 AT24CXX_Init();//EEPROM³õÊ¼»¯	 
	 delay_ms(1000);
	 delay_ms(1000);
	 PIN2_Init();
	 
	 
	 PID_INIT();//PID²ÎÊý³õÊ¼»¯
	 
	 LED_Init();
	 
	 printf("all ok\r\n");
	 
	 
	 AT24CXX_Read(0,iic_buf,6);//Ô¤ÏÈ¶ÁÈ¡Êý¾Ý
//	 
	// pid_kp=iic_buf[0]+(float)(iic_buf[1])/100.0;
	 //pid_ki=iic_buf[2]+(float)(iic_buf[3])/100.0;
	 //pid_kd=iic_buf[4]+(float)(iic_buf[5])/100.0;//ÕâÊÇµÚÒ»´ÎµÄ×ª»¯£¬ÀûÓÃEEPROM¾ÍÊÇÎªÁË±£Ö¤µôµç»¹ÄÜ¼ÍÂ¼Êý¾Ý
	 
	 printf(" chack ok!\r\n");
	 
  while(1)
	{	
		//printf("%c \r\n",(USART1->SR&0x20));
//		USART_RX_STA=0;//×¼±¸½ÓÊÜÊý¾Ý
//				//while(!(USART_RX_STA&0x4000));//{printf("ReciveData OK\r\n");delay_ms(10);};//{//printf("321\r\n");}
//				//delay_ms(200);
//				if(USART_RX_BUF[0]==0xFE)
//				{
//					for(testnum =0;testnum<9;testnum++)
//					{
//						testbuf[testnum]=USART_RX_BUF[testnum+1];//0xff×÷ÎªÊý¾Ý·¢ËÍµÄÒ»¸öÅÐ±ðÐÎÊ½
//						//printf("%d \r\n",testbuf[7]);
//						//USART_SendData(USART1,testbuf[testnum]);
//					}
//					
//					if(testbuf[0]==0xFF);
//						//printf("%d  %d \r\n",testbuf[0],testbuf[1]);
//					else if(testbuf[0]==0xFC)
//						printf("grasping\r\n");
//					//USART_SendData(USART1,testbuf[0]);
//					
//				}
		
		
		
		if(tim2flag)//¿ØÖÆÖÜÆÚ
		{
	  //adc15=Get_Adc_Average(ADC_Channel_15,10);
		//temp=(float)adc15*(3.3/4096);
		//angel_now = (temp/3.3)*300;
			
//			
//			if(SPEED_ROLL_FLAG==1)
//				angel_now=speed1;
//			else
//				angel_now=-1*speed1;
//			
//			if(MyTurnflag.RF_TURN)
//				MyControl.RightForward.angle_now=-1*MyControl.RightForward.angle_now;
//			if(!MyTurnflag.LF_TURN)
//				MyControl.LeftForward.angle_now=-1*MyControl.LeftForward.angle_now;
//			if(!MyTurnflag.LB_TURN)
//				MyControl.LeftBehind.angle_now=-1*MyControl.LeftBehind.angle_now;
//			if(!MyTurnflag.RB_TURN)
//				MyControl.RightBehind.angle_now=-1*MyControl.RightBehind.angle_now;			
			
			//printf("%f %f %f %f\r\n",MyControl.RightForward.angle_now,MyControl.LeftForward.angle_now,
						//	MyControl.LeftBehind.angle_now,MyControl.RightBehind.angle_now);
			
			
		//	printf("%f  ",MyControl.RightForward.angle_now);
			
			 //printf("%f \r\n",angel_now);
			//TIM2_PWM_OUTPUT(1500,1500,1500,1500);
			
//		adc14=Get_Adc_Average(ADC_Channel_14,10);
//		temp1=(float)adc14*(3.3/4096);
//		disire_angel = (temp1/3.3)*300;
		//printf("%f  %f\r\n",angel_now,disire_angel);
	//	printf("%f \r\n",disire_angel);
			
//			if(Angle_flag==0)
//			{
//				disire_angel=DisireAngle;
//			}
//			else
//			{
//				disire_angel=(pwmout1-500)/2000*300;
//			}
//			MyDisire.RightForward=60;
//			MyDisire.LeftForward=60;
//			MyDisire.LeftBehind=60;
//			MyDisire.RightBehind=60;
			
			//printf("%f  \r\n",MyControl.RightForward.angle_now);
//			PID_CALL(MyDisire);
			
		//	printf("%f  \r\n",MyControl.RightForward.angle_now);
			//disire_angel=testbuf[1]-90;
			//disire_angel=11;
			//PID_CAL(disire_angel);
			tim2flag=0;
			//printf("test1\r\n");
		 //TIM3_PWM_OUTPUT(1000,0,0,0);
		}
	}
		
 }
 
// void TIM2_IRQHandler(void)   //TIM2ÖÐ¶Ï
//{	
//	
//	//printf("what the fuck!\n");
//	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) //¼ì²éÖ¸¶¨µÄTIMÖÐ¶Ï·¢ÉúÓë·ñ:TIM ÖÐ¶ÏÔ´ 
//		{
//		TIM_ClearITPendingBit(TIM2, TIM_IT_Update  );  //Çå³ýTIMxµÄÖÐ¶Ï´ý´¦ÀíÎ»:TIM ÖÐ¶ÏÔ´ 
//		}
////	LED4_Tog;
//		
//	tim2flag++;//±êÖ¾±äÁ¿
//	
//}
 
  void TIM6_IRQHandler(void)   //TIM2ÖÐ¶Ï
{	
	
	//printf("what the fuck!\n");
	if (TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET) //¼ì²éÖ¸¶¨µÄTIMÖÐ¶Ï·¢ÉúÓë·ñ:TIM ÖÐ¶ÏÔ´ 
		{
		
		//PBout(5)=!PBout(5);
//	LED4_Tog;
		if(MyTurnflag.RF_TURN)
				MyControl.RightForward.angle_now=-1*MyControl.RightForward.angle_now;
			if(!MyTurnflag.LF_TURN)
				MyControl.LeftForward.angle_now=-1*MyControl.LeftForward.angle_now;
			if(!MyTurnflag.LB_TURN)
				MyControl.LeftBehind.angle_now=-1*MyControl.LeftBehind.angle_now;
			if(MyTurnflag.RB_TURN)
				MyControl.RightBehind.angle_now=-1*MyControl.RightBehind.angle_now;
			PBout(5)=!PBout(5);
			GetVelocity();
			
//			if(USART_RX_BUF[0]==0xfc)
//			{
//				velocity[0]=0;
//				velocity[1]=0;
//				velocity[2]=0;
//				controlflag=-1;
//			}
//			else if(USART_RX_BUF[0]==0xff)
//			{
//				USART_RX_STA=0;
//				delay_ms(12);
//				velocity[0]=(USART_RX_BUF[1]*256+USART_RX_BUF[2]-2000)*0.00025;
//				velocity[1]=(USART_RX_BUF[3]*256+USART_RX_BUF[4]-2000)*0.00025;
//				//justtest=USART_RX_BUF[0]*256+USART_RX_BUF[1];
//				velocity[2]=(USART_RX_BUF[5]*256+USART_RX_BUF[6]-2000)*0.00025;
//				printf("%f \r\n",velocity[1]);
//				
//			}
			
			GetDisire();
			
			//printf("%f \r\n",MyControl.RightBehind.angle_now);
			
			//MyDisire.RightForward=40;
			//MyDisire.LeftForward=60;
			//MyDisire.LeftBehind=50;
			//MyDisire.RightBehind=-40;
			
			PID_CALL(MyDisire);
		
		
			tim2flag++;//±êÖ¾±äÁ
			TIM_ClearITPendingBit(TIM6, TIM_IT_Update  );  //Çå³ýTIMxµÄÖÐ¶Ï´ý´¦ÀíÎ»:TIM ÖÐ¶ÏÔ´ 
		}
	
}
