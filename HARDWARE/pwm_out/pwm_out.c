/********************************************************************************
 * �ļ���  ��pwm_output.c
 * ����    ��         
 * ʵ��ƽ̨��STM32F103ZET
 * PWM�ӿ� 		
 * ��汾  ��3.5.0
 * ����    ������
 * ʱ��    ��2015��8��20��10:29:20 
**********************************************************************************/

//ע�⣬�����õ���TIM5�ģ����ǲ�֪��ΪʲôTIM5���������⣬û�취���ĳ�TIM3��������

#include  "pwm_out.h"


///*
// * ��������TIM5_GPIO_Config
// * ����  ������TIM5�������PWMʱ�õ���I/O
// * ����  ����
// * ���  ����
// * ����  ���ڲ�����
// */
// 
// 
// //PA0,PA1,PA2,PA3 TIM5���ĸ�ͨ��
//static void TIM5_GPIO_Config(void) 
//{
//  GPIO_InitTypeDef GPIO_InitStructure;   //����������������
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);	//ʹ�ܶ�ʱ��5ʱ��
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);  //ʹ��GPIO�����AFIO���ù���ģ��ʱ��
//	
//	
//	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3 ;//��������
//	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;//�����������
//	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;//����ת��Ƶ��50MHz
//	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIO

//}

///*
// * ��������TIM5_Mode_Config
// * ����  ������TIM5��ʱ���Ĺ���ģʽ
// * ����  ����
// * ���  ����
// * ����  ���ڲ�����
// */

//static void TIM5_Mode_Config(u16 CCR1_Val,u16 CCR2_Val,u16 CCR3_Val,u16 CCR4_Val)//�ڲ����������ڲ�����
//{
//	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
//	TIM_OCInitTypeDef  TIM_OCInitStructure;
//	
//	TIM_TimeBaseStructure.TIM_Period = 20000-1; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ���������PWM������Ϊ8MS
//	TIM_TimeBaseStructure.TIM_Prescaler =72-1; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ������Ԥ��Ƶλ720 
//	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
//	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
//	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
//	
//	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	    //����ΪPWMģʽ1
//  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	
//  TIM_OCInitStructure.TIM_Pulse = 0;	   //��������ֵ�������������������ֵʱ����ƽ��������
//  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  //����ʱ������ֵС��CCR1_ValʱΪ�ߵ�ƽ
//  
//	
//	TIM_OC1Init(TIM5, &TIM_OCInitStructure);	 //ʹ��ͨ��1
//  TIM_OC1PreloadConfig(TIM5, TIM_OCPreload_Enable);
//	TIM_OC2Init(TIM5, &TIM_OCInitStructure);	 //ʹ��ͨ��2
//  TIM_OC2PreloadConfig(TIM5, TIM_OCPreload_Enable);
//  TIM_OC3Init(TIM5, &TIM_OCInitStructure);	 //ʹ��ͨ��3
//  TIM_OC3PreloadConfig(TIM5, TIM_OCPreload_Enable);
//  TIM_OC4Init(TIM5, &TIM_OCInitStructure);	 //ʹ��ͨ��4
//  TIM_OC4PreloadConfig(TIM5, TIM_OCPreload_Enable);
//	
//	TIM_ARRPreloadConfig(TIM5, ENABLE);			 // ʹ��TIM5���ؼĴ���ARR
//	
//  TIM_Cmd(TIM5, ENABLE);                   //ʹ�ܶ�ʱ��5
//}

/*
 * ��������TIM5_Mode_Config
 * ����  ��TIM5 ���PWM�źų�ʼ����ֻҪ�����������
 *         TIM5���ĸ�ͨ���ͻ���PWM�ź����
 * ����  ����
 * ���  ����
 * ����  ���ⲿ����
 */
//void TIM5_PWM_Init(void)
//{
//	TIM5_GPIO_Config();
//	TIM5_Mode_Config(0,0,0,0);	
//}


void TIM3_PWM_Init(u16 arr,u16 psc)//
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC  | RCC_APB2Periph_AFIO, ENABLE);  //ʹ��GPIO�����AFIO���ù���ģ��ʱ��ʹ��
	
//	GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE); //Timer3������ӳ��  TIM3_CH2->PB5                                                                       	 //����TIM3��CH2�����PWMͨ����LED��ʾ
// 
//   //���ø�����Ϊ�����������,���TIM3 CH2��PWM���岨��
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1; //TIM3_CH3+TIM3_CH4
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOB, &GPIO_InitStructure);
//	//GPIO_WriteBit(GPIOA, GPIO_Pin_7,Bit_SET); // PA7����	
//	
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5; //TIM3_CH1+TIM3_CH2
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_PinRemapConfig(GPIO_FullRemap_TIM3, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9; //TIM3_CH3+TIM3_CH4
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_SetBits(GPIOC, GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9); // PA7����

	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 80K
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  ����Ƶ
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
	
	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_Pulse = 0; //���ô�װ�벶��ȽϼĴ���������ֵ
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ը�
	
	TIM_OC1Init(TIM3, &TIM_OCInitStructure);  //����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);  //ʹ��TIMx��CCR2�ϵ�Ԥװ�ؼĴ���
	
	TIM_OC2Init(TIM3, &TIM_OCInitStructure);  //����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);  //ʹ��TIMx��CCR2�ϵ�Ԥװ�ؼĴ���
	
	TIM_OC3Init(TIM3, &TIM_OCInitStructure);  //����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx
	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);  //ʹ��TIMx��CCR2�ϵ�Ԥװ�ؼĴ���
	
	TIM_OC4Init(TIM3, &TIM_OCInitStructure);  //����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx
	TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);  //ʹ��TIMx��CCR2�ϵ�Ԥװ�ؼĴ���
	
	TIM_ARRPreloadConfig(TIM3, ENABLE); //ʹ��TIMx��ARR�ϵ�Ԥװ�ؼĴ���
	
 
	TIM_Cmd(TIM3, ENABLE);  //ʹ��TIMx����
 

}
/*
 * ��������TIM5_PWM_OUTPUT
 * ����  ��TIM5����ɿ�PWM�ź�
 * ����  ��DR1
 * ���  ����
 * ����	 ���ⲿ����		   
 */

void TIM3_PWM_OUTPUT(u16 DR1,u16 DR2,u16 DR3,u16 DR4)
{
	TIM_SetCompare1(TIM3,DR1);
	TIM_SetCompare2(TIM3,DR2);
	TIM_SetCompare3(TIM3,DR3);
	TIM_SetCompare4(TIM3,DR4);
}



//��ʱ��2Ҳ�����������
void TIM2_PWM_Init(u16 arr,u16 psc)//
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA  | RCC_APB2Periph_AFIO, ENABLE);  //ʹ��GPIO�����AFIO���ù���ģ��ʱ��ʹ��
	
//	GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE); //Timer3������ӳ��  TIM3_CH2->PB5                                                                       	 //����TIM3��CH2�����PWMͨ����LED��ʾ
 
   //���ø�����Ϊ�����������,���TIM3 CH2��PWM���岨��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3; //TIM3_CH3+TIM3_CH4
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//GPIO_WriteBit(GPIOA, GPIO_Pin_7,Bit_SET); // PA7����	
	

	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 80K
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  ����Ƶ
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
	
	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_Pulse = 0; //���ô�װ�벶��ȽϼĴ���������ֵ
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ը�
	
	TIM_OC1Init(TIM2, &TIM_OCInitStructure);  //����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx
	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);  //ʹ��TIMx��CCR2�ϵ�Ԥװ�ؼĴ���
	
	TIM_OC2Init(TIM2, &TIM_OCInitStructure);  //����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx
	TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);  //ʹ��TIMx��CCR2�ϵ�Ԥװ�ؼĴ���
	
	TIM_OC3Init(TIM2, &TIM_OCInitStructure);  //����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx
	TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);  //ʹ��TIMx��CCR2�ϵ�Ԥװ�ؼĴ���
	
	TIM_OC4Init(TIM2, &TIM_OCInitStructure);  //����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx
	TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);  //ʹ��TIMx��CCR2�ϵ�Ԥװ�ؼĴ���
	
	TIM_ARRPreloadConfig(TIM2, ENABLE); //ʹ��TIMx��ARR�ϵ�Ԥװ�ؼĴ���
	
 
	TIM_Cmd(TIM2, ENABLE);  //ʹ��TIMx����
 
}

void TIM2_PWM_OUTPUT(u16 DR1,u16 DR2,u16 DR3,u16 DR4)
{
	TIM_SetCompare1(TIM2,DR1);
	TIM_SetCompare2(TIM2,DR2);
	TIM_SetCompare3(TIM2,DR3);
	TIM_SetCompare4(TIM2,DR4);
}

void TIM_PWM_SET(u16 DR1,u16 DR2,u16 DR3,u16 DR4,u16 DR5,u16 DR6,u16 DR7,u16 DR8)
{
	TIM2_PWM_OUTPUT(DR1,DR2,DR3,DR4);
	TIM3_PWM_OUTPUT(DR5,DR6,DR7,DR8);
}







////���������������������������̨�ģ���ʱ���ò�����������������Ӧ����TIM4�ĵ�����ΪTIM4��֧��TF�ı���ʱ����ȥ�����ջ���


////ֻ����TIM5��
//static void TIM5_GPIO_Config(void)
//{
//	
//  GPIO_InitTypeDef GPIO_InitStructure;   //����������������
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);	//ʹ�ܶ�ʱ��4ʱ��
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);  //ʹ��GPIO�����AFIO���ù���ģ��ʱ��
//	
//	
//	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;//��������
//	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;//�����������
//	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;//����ת��Ƶ��50MHz
//	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIO

//}



//static void TIM5_Mode_Config(void)
//{
//	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
//	TIM_OCInitTypeDef  TIM_OCInitStructure;

//  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);  
//	
//	/* Time base configuration */		 
//  TIM_TimeBaseStructure.TIM_Period = 25000-1;      //PWM����
//  TIM_TimeBaseStructure.TIM_Prescaler = 72-1;	    //����Ԥ��Ƶ����Ϊ1MHz
//  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;	//����ʱ�ӷ�Ƶϵ��������Ƶ
//  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //���ϼ���ģʽ

//  TIM_TimeBaseStructure.TIM_Period = 20000-1;  //PWM����

//  TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);
//  TIM_ARRPreloadConfig(TIM5, ENABLE);

//  /* PWM1 Mode configuration: Channel1 */
//  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	    //����ΪPWMģʽ1
//  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	
//  TIM_OCInitStructure.TIM_Pulse = 1500;	   //��������ֵ�������������������ֵʱ����ƽ��������
//  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  //����ʱ������ֵС��CCR1_ValʱΪ�ߵ�ƽ

//	 TIM_OC1Init(TIM5, &TIM_OCInitStructure);	 //ʹ��ͨ��3
//  TIM_OC1PreloadConfig(TIM5, TIM_OCPreload_Enable);
//	 TIM_OC2Init(TIM5, &TIM_OCInitStructure);	 //ʹ��ͨ��3
//  TIM_OC2PreloadConfig(TIM5, TIM_OCPreload_Enable);

//  TIM_OC3Init(TIM5, &TIM_OCInitStructure);	 //ʹ��ͨ��3
//  TIM_OC3PreloadConfig(TIM5, TIM_OCPreload_Enable);
//  TIM_OC4Init(TIM5, &TIM_OCInitStructure);	 //ʹ��ͨ��4
//  TIM_OC4PreloadConfig(TIM5, TIM_OCPreload_Enable);

//  TIM_ARRPreloadConfig(TIM5, ENABLE);			 // ʹ��TIM5���ؼĴ���ARR
//	
//  TIM_Cmd(TIM5, ENABLE);                   //ʹ�ܶ�ʱ��4
//}

//void TIM5_PWM_Init(void)
//{
//	TIM5_GPIO_Config();
//	TIM5_Mode_Config();	
//}

//void TIM5_PWM_OUTPUT(u16 DR3,u16 DR4)
//{
//	//TIM_SetCompare1(TIM4,DR1);
//	//TIM_SetCompare2(TIM4,DR2);
//	TIM_SetCompare3(TIM5,DR3);
//	TIM_SetCompare4(TIM5,DR4);
//}

//void sound_pwm(u16 DR1)
//{
//	TIM_SetCompare1(TIM5,DR1);
//}

