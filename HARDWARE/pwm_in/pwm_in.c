/********************************************************************************
* �ļ���  ��pwm_output.c
* ����    ��
* ʵ��ƽ̨��STM32F103ZET
* PWM�ӿ�
* ��汾  ��3.5.0
* ����    ������
* ʱ��    ��2015��8��21��23:26:26
**********************************************************************************/


#include "pwm_in.h"
#include "sys.h"
#include "usart.h"
#include "pid.h"

//���Դ�ļ�Ŀǰ���ò������Ժ��õģ����ǽ���ң�������������ݵģ����Ժ�����ң������˵��

//����ļ������ע�⣬TIM5�ǲ�������ֱ������5V�� ����TIM4ȴ֧�֣�����һ��Ҫʹ��TIM4��Ϊ�����źŵĶ�ʱ������Ȼ�ͻ��ջ�оƬ

u8  TIM4CH1_CAPTURE_STA = 0;    //ͨ��1���벶��״̬       �ø���λ�������־������λ���������
int TIM4CH1_CAPTURE_UPVAL;  //ͨ��1���벶��ֵ
int TIM4CH1_CAPTURE_DOWNVAL;    //ͨ��1���벶��ֵ

u8  TIM4CH2_CAPTURE_STA = 0;    //ͨ��2���벶��״̬
int TIM4CH2_CAPTURE_UPVAL;  //ͨ��2���벶��ֵ
int TIM4CH2_CAPTURE_DOWNVAL;    //ͨ��2���벶��ֵ

u8  TIM4CH3_CAPTURE_STA = 0;    //ͨ��3���벶��״̬
int TIM4CH3_CAPTURE_UPVAL;  //ͨ��3���벶��ֵ
int TIM4CH3_CAPTURE_DOWNVAL;    //ͨ��3���벶��ֵ

u8  TIM4CH4_CAPTURE_STA = 0;    //ͨ��4���벶��״̬
int TIM4CH4_CAPTURE_UPVAL;  //ͨ��4���벶��ֵ
int TIM4CH4_CAPTURE_DOWNVAL;    //ͨ��4���벶��ֵ


u8 SPEED_ROLL_FLAG=1;
TURNFLAG MyTurnflag ={1,1,1,1};//ת��ı�־λ


int tempup1 = 0;//�����ܸߵ�ƽ��ʱ��us��λ
int tempup2 = 0;//�����ܸߵ�ƽ��ʱ��us��λ
int tempup3 = 0;//�����ܸߵ�ƽ��ʱ��us��λ
int tempup4 = 0;//�����ܸߵ�ƽ��ʱ��us��λ
u32 tim3_T;

int pwmout1, pwmout2, pwmout3, pwmout4;                 //���ռ�ձ�


int speed1;

unsigned char Control_Mode;



//�ټ���һ��TIM1�Ĳ������룬�����õ���������ֵ��

u8  TIM1CH1_CAPTURE_STA = 0;    //ͨ��1���벶��״̬       �ø���λ�������־������λ���������
int TIM1CH1_CAPTURE_UPVAL;  //ͨ��1���벶��ֵ
int TIM1CH1_CAPTURE_DOWNVAL;    //ͨ��1���벶��ֵ

u8  TIM1CH2_CAPTURE_STA = 0;    //ͨ��2���벶��״̬
int TIM1CH2_CAPTURE_UPVAL;  //ͨ��2���벶��ֵ
int TIM1CH2_CAPTURE_DOWNVAL;    //ͨ��2���벶��ֵ

u8  TIM1CH3_CAPTURE_STA = 0;    //ͨ��3���벶��״̬
int TIM1CH3_CAPTURE_UPVAL;  //ͨ��3���벶��ֵ
int TIM1CH3_CAPTURE_DOWNVAL;    //ͨ��3���벶��ֵ

u8  TIM1CH4_CAPTURE_STA = 0;    //ͨ��4���벶��״̬
int TIM1CH4_CAPTURE_UPVAL;  //ͨ��4���벶��ֵ
int TIM1CH4_CAPTURE_DOWNVAL;    //ͨ��4���벶��ֵ

u8 whatup;


int tempup11 = 0;//�����ܸߵ�ƽ��ʱ��us��λ
int tempup12 = 0;//�����ܸߵ�ƽ��ʱ��us��λ
int tempup13 = 0;//�����ܸߵ�ƽ��ʱ��us��λ
int tempup14 = 0;//�����ܸߵ�ƽ��ʱ��us��λ

int pwmout11, pwmout12, pwmout13, pwmout14;                 //���ռ�ձ�


//TIM4��ʱ��ƥ�䣬���ڼ��ң����ͨ���������ź�

void pwm_in_init(void)//��ʱ��4�����ڽ���ң�������ź�
{
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_ICInitTypeDef  TIM4_ICInitStructure;
    //  EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);    //ʹ�ܶ�ʱ��4ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);  //ʹ��GPIO�����AFIO���ù���ģ��ʱ�ӣ�������ӳ�䣬�ŵ�D�˿�
	
		GPIO_PinRemapConfig(GPIO_Remap_TIM4, ENABLE); //������ӳ�䣬��Ϊǰ���ʱ��ͷ������г�ͻ�ܳ�
	
//    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;  //PA0 ���֮ǰ����
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; // ����
//    GPIO_Init(GPIOB, &GPIO_InitStructure);
//    GPIO_ResetBits(GPIOB, GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9);                        // ����
	
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;  //PA0 ���֮ǰ����
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; // ����
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    GPIO_ResetBits(GPIOD, GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);   
	
	
    //  GPIO_InitStructure1.GPIO_Pin  = GPIO_Pin_0|GPIO_Pin_1;  //PA0 ���֮ǰ����
    //  GPIO_InitStructure1.GPIO_Mode = GPIO_Mode_IPD; //PA0 ����
    //  GPIO_Init(GPIOB, &GPIO_InitStructure1);
    //  GPIO_ResetBits(GPIOB,GPIO_Pin_0|GPIO_Pin_1);                         //PA0 ����
    //--------------------------------------------------���ö�ʱ��5���벶��--------------------------------------------
	
	
	
    //��ʼ����ʱ��4 TIM4
    TIM_TimeBaseStructure.TIM_Period = 0XFFFF; //�趨�������Զ���װֵ
    TIM_TimeBaseStructure.TIM_Prescaler = 72 - 1;   //Ԥ��Ƶ��
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
    //��ʼ��TIM3���벶�����
    TIM4_ICInitStructure.TIM_Channel = TIM_Channel_1; //CC1S=01     ѡ������� IC1ӳ�䵽TI1��
    TIM4_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;    //�����ز���
    TIM4_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //ӳ�䵽TI1��
    TIM4_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;   //���������Ƶ,����Ƶ
    TIM4_ICInitStructure.TIM_ICFilter = 0x00;//IC1F=0000 ���������˲��� ���˲�
    TIM_ICInit(TIM4, &TIM4_ICInitStructure);
    TIM4_ICInitStructure.TIM_Channel = TIM_Channel_2; //CC1S=01     ѡ������� IC1ӳ�䵽TI1��
    TIM4_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;    //�����ز���
    TIM4_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //ӳ�䵽TI1��
    TIM4_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;   //���������Ƶ,����Ƶ
    TIM4_ICInitStructure.TIM_ICFilter = 0x00;//IC1F=0000 ���������˲��� ���˲�
    TIM_ICInit(TIM4, &TIM4_ICInitStructure);
    TIM4_ICInitStructure.TIM_Channel = TIM_Channel_3; //CC1S=01     ѡ������� IC1ӳ�䵽TI1��
    TIM4_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;    //�����ز���
    TIM4_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //ӳ�䵽TI1��
    TIM4_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;   //���������Ƶ,����Ƶ
    TIM4_ICInitStructure.TIM_ICFilter = 0x00;//IC1F=0000 ���������˲��� ���˲�
    TIM_ICInit(TIM4, &TIM4_ICInitStructure);
    TIM4_ICInitStructure.TIM_Channel = TIM_Channel_4; //CC1S=01     ѡ������� IC1ӳ�䵽TI1��
    TIM4_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;    //�����ز���
    TIM4_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //ӳ�䵽TI1��
    TIM4_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;   //���������Ƶ,����Ƶ
    TIM4_ICInitStructure.TIM_ICFilter = 0x00;//IC1F=0000 ���������˲��� ���˲�
    TIM_ICInit(TIM4, &TIM4_ICInitStructure);
    //�жϷ����ʼ��
    NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  //TIM5�ж�
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //��ռ���ȼ�2��
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  //�����ȼ�1��
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
    NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���
    TIM_ITConfig(TIM4, TIM_IT_Update | TIM_IT_CC1, ENABLE);//��������ж� ,����CC1IE�����ж�
		
		
    TIM_ITConfig(TIM4, TIM_IT_CC1, ENABLE);//��������ж� ,����CC1IE�����ж�
    TIM_ITConfig(TIM4, TIM_IT_CC2, ENABLE);//��������ж� ,����CC1IE�����ж�
    TIM_ITConfig(TIM4, TIM_IT_CC3, ENABLE);//��������ж� ,����CC1IE�����ж�
    TIM_ITConfig(TIM4, TIM_IT_CC4, ENABLE);//��������ж� ,����CC1IE�����ж�
		
    TIM_Cmd(TIM4, ENABLE);  //ʹ�ܶ�ʱ��4
}





//��ʱ��4�жϷ������
void TIM4_IRQHandler(void)
{
    if((TIM4CH1_CAPTURE_STA & 0X80) == 0) //��δ�ɹ�����
    {
        if(TIM_GetITStatus(TIM4, TIM_IT_CC1) != RESET) //����1���������¼�
        {
            if(TIM4CH1_CAPTURE_STA & 0X40)      //����һ���½���
            {
                TIM4CH1_CAPTURE_STA |= 0X80;        //��ǳɹ�����һ�θߵ�ƽ����
                TIM4CH1_CAPTURE_DOWNVAL = TIM_GetCapture1(TIM4);
                TIM_OC1PolarityConfig(TIM4, TIM_ICPolarity_Rising); //CC1P=0 ����Ϊ�����ز���

                if(TIM4CH1_CAPTURE_DOWNVAL < TIM4CH1_CAPTURE_UPVAL)
                {
                    tempup1 = 65536 - (TIM4CH1_CAPTURE_UPVAL - TIM4CH1_CAPTURE_DOWNVAL);
                }
                else
                {
                    tempup1 = TIM4CH1_CAPTURE_DOWNVAL - TIM4CH1_CAPTURE_UPVAL;
                }
								pwmout1=tempup1;
                //printf("1  HIGH:%d us\r\n", tempup1);//����ߵ�ƽʱ��
                TIM4CH1_CAPTURE_STA = 0;//������һ�β���
            }
            else                                //��δ��ʼ,��һ�β���������
            {
                TIM4CH1_CAPTURE_STA = 0;            //���
                TIM4CH1_CAPTURE_UPVAL = TIM_GetCapture1(TIM4);
                TIM4CH1_CAPTURE_STA |= 0X40;        //��ǲ�����������
                TIM_OC1PolarityConfig(TIM4, TIM_ICPolarity_Falling);        //CC1P=1 ����Ϊ�½��ز���
            }
        }
    }

    if((TIM4CH2_CAPTURE_STA & 0X80) == 0) //��δ�ɹ�����
    {
        if(TIM_GetITStatus(TIM4, TIM_IT_CC2) != RESET) //����1���������¼�
        {		
            if(TIM4CH2_CAPTURE_STA & 0X40)      //����һ���½���
            {
                TIM4CH2_CAPTURE_STA |= 0X80;        //��ǳɹ�����һ�θߵ�ƽ����
                TIM4CH2_CAPTURE_DOWNVAL = TIM_GetCapture2(TIM4);
                TIM_OC2PolarityConfig(TIM4, TIM_ICPolarity_Rising); //CC1P=0 ����Ϊ�����ز���

                if(TIM4CH2_CAPTURE_DOWNVAL < TIM4CH2_CAPTURE_UPVAL)
                {
                    tempup2 = 65536 - (TIM4CH2_CAPTURE_UPVAL - TIM4CH2_CAPTURE_DOWNVAL);
                }
                else
                {
                    tempup2 = TIM4CH2_CAPTURE_DOWNVAL - TIM4CH2_CAPTURE_UPVAL;
                }
								pwmout2=tempup2;
                //printf("2  HIGH:%d us\r\n", tempup2);
                TIM4CH2_CAPTURE_STA = 0;//������һ�β���
            }
            else                                //��δ��ʼ,��һ�β���������
            {
                TIM4CH2_CAPTURE_STA = 0;            //���
                TIM4CH2_CAPTURE_UPVAL = TIM_GetCapture2(TIM4);
                TIM4CH2_CAPTURE_STA |= 0X40;        //��ǲ�����������
                TIM_OC2PolarityConfig(TIM4, TIM_ICPolarity_Falling);        //CC1P=1 ����Ϊ�½��ز���
            }
        }
    }

    if((TIM4CH3_CAPTURE_STA & 0X80) == 0) //��δ�ɹ�����
    {
        if(TIM_GetITStatus(TIM4, TIM_IT_CC3) != RESET) //����1���������¼�
        {		
            if(TIM4CH3_CAPTURE_STA & 0X40)      //����һ���½���
            {
                TIM4CH3_CAPTURE_STA |= 0X80;        //��ǳɹ�����һ�θߵ�ƽ����
                TIM4CH3_CAPTURE_DOWNVAL = TIM_GetCapture3(TIM4);
                TIM_OC3PolarityConfig(TIM4, TIM_ICPolarity_Rising); //CC1P=0 ����Ϊ�����ز���

                if(TIM4CH3_CAPTURE_DOWNVAL < TIM4CH3_CAPTURE_UPVAL)
                {
                    tempup3 = 65536 - (TIM4CH3_CAPTURE_UPVAL - TIM4CH3_CAPTURE_DOWNVAL);
                }
                else
                {
                    tempup3 = TIM4CH3_CAPTURE_DOWNVAL - TIM4CH3_CAPTURE_UPVAL;
                }
								pwmout3=tempup3;
               // printf("3  HIGH:%d us\r\n", tempup3);
                TIM4CH3_CAPTURE_STA = 0;//������һ�β���
            }
            else                                //��δ��ʼ,��һ�β���������
            {
                TIM4CH3_CAPTURE_STA = 0;            //���
                TIM4CH3_CAPTURE_UPVAL = TIM_GetCapture3(TIM4);
                TIM4CH3_CAPTURE_STA |= 0X40;        //��ǲ�����������
                TIM_OC3PolarityConfig(TIM4, TIM_ICPolarity_Falling);        //CC1P=1 ����Ϊ�½��ز���
            }
        }
    }
    if((TIM4CH4_CAPTURE_STA & 0X80) == 0) //��δ�ɹ�����
    {
        if(TIM_GetITStatus(TIM4, TIM_IT_CC4) != RESET) //����1���������¼�
        {		
            if(TIM4CH4_CAPTURE_STA & 0X40)      //����һ���½���
            {
                TIM4CH4_CAPTURE_STA |= 0X80;        //��ǳɹ�����һ�θߵ�ƽ����
                TIM4CH4_CAPTURE_DOWNVAL = TIM_GetCapture4(TIM4);
                TIM_OC4PolarityConfig(TIM4, TIM_ICPolarity_Rising); //CC1P=0 ����Ϊ�����ز���

                if(TIM4CH4_CAPTURE_DOWNVAL < TIM4CH4_CAPTURE_UPVAL)
                {
                    tempup4 = 65536 - (TIM4CH4_CAPTURE_UPVAL - TIM4CH4_CAPTURE_DOWNVAL);
                }
                else
                {
                    tempup4 = TIM4CH4_CAPTURE_DOWNVAL - TIM4CH4_CAPTURE_UPVAL;
                }
								pwmout4=tempup4;
               // printf("4  HIGH:%d us\r\n", tempup4);
                TIM4CH4_CAPTURE_STA = 0;//������һ�β���
            }
            else                                //��δ��ʼ,��һ�β���������
            {
                TIM4CH4_CAPTURE_STA = 0;            //���
                TIM4CH4_CAPTURE_UPVAL = TIM_GetCapture4(TIM4);
                TIM4CH4_CAPTURE_STA |= 0X40;        //��ǲ�����������
                TIM_OC4PolarityConfig(TIM4, TIM_ICPolarity_Falling);        //CC1P=1 ����Ϊ�½��ز���
            }
        }
    }

    TIM_ClearITPendingBit(TIM4, TIM_IT_CC1 | TIM_IT_CC2 | TIM_IT_CC3 | TIM_IT_CC4 | TIM_IT_Update); //����жϱ�־λ
}






void PIN2_Init()//��ʼ�����ţ����ڲ��������ת��
{
	GPIO_InitTypeDef  GPIO_InitStructure;
 	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE|RCC_APB2Periph_GPIOF, ENABLE);	 //ʹ��PB,PE�˿�ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF|RCC_APB2Periph_GPIOF, ENABLE);	 //ʹ��PB,PE�˿�ʱ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4;//|GPIO_Pin_6;				 //LED0-->PB.5 �˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 		 //�������
	//GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
	GPIO_Init(GPIOE, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOB.5
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;				 //LED0-->PB.5 �˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 		 //�������
	//GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
	GPIO_Init(GPIOF, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOB.5
	
	//GPIO_SetBits(GPIOE,GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5);						 //PB.5 �����
	
}



void tim1_pwm_in_init(void)//TIM1���벶�����ڲ�������ٶ�
{
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_ICInitTypeDef  TIM1_ICInitStructure;
    //  EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);    //ʹ�ܶ�ʱ��1ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE | RCC_APB2Periph_AFIO, ENABLE);  //ʹ��GPIO�����AFIO���ù���ģ��ʱ�ӣ�������ӳ�䣬�ŵ�D�˿�
	
		GPIO_PinRemapConfig(GPIO_FullRemap_TIM1, ENABLE); 
	
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_11 | GPIO_Pin_13 | GPIO_Pin_14;  //PA0 ���֮ǰ����
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; // ����
    GPIO_Init(GPIOE, &GPIO_InitStructure);//������Ҫע��TIM1������ӳ���Ժ�Ķ�Ӧ�˿�ʵ��PE�˿ڵĲ�Ҫ�����
    GPIO_ResetBits(GPIOE, GPIO_Pin_9 | GPIO_Pin_11 | GPIO_Pin_13 | GPIO_Pin_14);                        // ����
	
	
	
	
    //  GPIO_InitStructure1.GPIO_Pin  = GPIO_Pin_0|GPIO_Pin_1;  //PA0 ���֮ǰ����
    //  GPIO_InitStructure1.GPIO_Mode = GPIO_Mode_IPD; //PA0 ����
    //  GPIO_Init(GPIOB, &GPIO_InitStructure1);
    //  GPIO_ResetBits(GPIOB,GPIO_Pin_0|GPIO_Pin_1);                         //PA0 ����
    //--------------------------------------------------���ö�ʱ��5���벶��--------------------------------------------
	
	
	
    //��ʼ����ʱ��1 TIM1
    TIM_TimeBaseStructure.TIM_Period = 0XFFFF; //�趨�������Զ���װֵ
    TIM_TimeBaseStructure.TIM_Prescaler = 72 - 1;   //Ԥ��Ƶ��
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
    //��ʼ��TIM3���벶�����
    TIM1_ICInitStructure.TIM_Channel = TIM_Channel_1; //CC1S=01     ѡ������� IC1ӳ�䵽TI1��
    TIM1_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;    //�����ز���
    TIM1_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //ӳ�䵽TI1��
    TIM1_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;   //���������Ƶ,����Ƶ
    TIM1_ICInitStructure.TIM_ICFilter = 0x00;//IC1F=0000 ���������˲��� ���˲�
    TIM_ICInit(TIM1, &TIM1_ICInitStructure);
    TIM1_ICInitStructure.TIM_Channel = TIM_Channel_2; //CC1S=01     ѡ������� IC1ӳ�䵽TI1��
    TIM1_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;    //�����ز���
    TIM1_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //ӳ�䵽TI1��
    TIM1_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;   //���������Ƶ,����Ƶ
    TIM1_ICInitStructure.TIM_ICFilter = 0x00;//IC1F=0000 ���������˲��� ���˲�
    TIM_ICInit(TIM1, &TIM1_ICInitStructure);
    TIM1_ICInitStructure.TIM_Channel = TIM_Channel_3; //CC1S=01     ѡ������� IC1ӳ�䵽TI1��
    TIM1_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;    //�����ز���
    TIM1_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //ӳ�䵽TI1��
    TIM1_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;   //���������Ƶ,����Ƶ
    TIM1_ICInitStructure.TIM_ICFilter = 0x00;//IC1F=0000 ���������˲��� ���˲�
    TIM_ICInit(TIM1, &TIM1_ICInitStructure);
    TIM1_ICInitStructure.TIM_Channel = TIM_Channel_4; //CC1S=01     ѡ������� IC1ӳ�䵽TI1��
    TIM1_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;    //�����ز���
    TIM1_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //ӳ�䵽TI1��
    TIM1_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;   //���������Ƶ,����Ƶ
    TIM1_ICInitStructure.TIM_ICFilter = 0x00;//IC1F=0000 ���������˲��� ���˲�
    TIM_ICInit(TIM1, &TIM1_ICInitStructure);
    //�жϷ����ʼ��
    NVIC_InitStructure.NVIC_IRQChannel = TIM1_CC_IRQn;  //TIM1�����ж�
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;  //��ռ���ȼ�2��
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�����ȼ�0��
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
    NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���
    TIM_ITConfig(TIM1, TIM_IT_Update | TIM_IT_CC1, ENABLE);//��������ж� ,����CC1IE�����ж�
		
		
    TIM_ITConfig(TIM1, TIM_IT_CC1, ENABLE);//��������ж� ,����CC1IE�����ж�
    TIM_ITConfig(TIM1, TIM_IT_CC2, ENABLE);//��������ж� ,����CC1IE�����ж�
    TIM_ITConfig(TIM1, TIM_IT_CC3, ENABLE);//��������ж� ,����CC1IE�����ж�
    TIM_ITConfig(TIM1, TIM_IT_CC4, ENABLE);//��������ж� ,����CC1IE�����ж�
		
    TIM_Cmd(TIM1, ENABLE);  //ʹ�ܶ�ʱ��4
}


//��ʱ��1�жϷ������
void TIM1_CC_IRQHandler(void)
{
    if((TIM1CH1_CAPTURE_STA & 0X80) == 0) //��δ�ɹ�����
    {
        if(TIM_GetITStatus(TIM1, TIM_IT_CC1) != RESET) //����1���������¼�
        {
            if(TIM1CH1_CAPTURE_STA & 0X40)      //����һ���½���
            {
                TIM1CH1_CAPTURE_STA |= 0X80;        //��ǳɹ�����һ�θߵ�ƽ����
                TIM1CH1_CAPTURE_DOWNVAL = TIM_GetCapture1(TIM1);
                TIM_OC1PolarityConfig(TIM1, TIM_ICPolarity_Rising); //CC1P=0 ����Ϊ�����ز���

                if(TIM1CH1_CAPTURE_DOWNVAL < TIM1CH1_CAPTURE_UPVAL)
                {
                    tempup11 = 65536 - (TIM1CH1_CAPTURE_UPVAL - TIM1CH1_CAPTURE_DOWNVAL);
                }
                else
                {
                    tempup11 = TIM1CH1_CAPTURE_DOWNVAL - TIM1CH1_CAPTURE_UPVAL;
                }
								pwmout11=tempup11;
								MyControl.RightForward.angle_now= 78125/tempup11/2;
								//printf("SPEED: %f \r\n",MyControl.RightForward.angle_now);
								if(PEin(2))
									MyTurnflag.RF_TURN =1;
								else
									MyTurnflag.RF_TURN =0;
                //printf("HIGH:%d us\r\n", tempup11);
                TIM1CH1_CAPTURE_STA = 0;//������һ�β���
            }
            else                                //��δ��ʼ,��һ�β���������
            {
                TIM1CH1_CAPTURE_STA = 0;            //���
                TIM1CH1_CAPTURE_UPVAL = TIM_GetCapture1(TIM1);
                TIM1CH1_CAPTURE_STA |= 0X40;        //��ǲ�����������
                TIM_OC1PolarityConfig(TIM1, TIM_ICPolarity_Falling);        //CC1P=1 ����Ϊ�½��ز���
            }
        }
    }

    if((TIM1CH2_CAPTURE_STA & 0X80) == 0) //��δ�ɹ�����
    {
        if(TIM_GetITStatus(TIM1, TIM_IT_CC2) != RESET) //����1���������¼�
        {		
            if(TIM1CH2_CAPTURE_STA & 0X40)      //����һ���½���
            {
                TIM1CH2_CAPTURE_STA |= 0X80;        //��ǳɹ�����һ�θߵ�ƽ����
                TIM1CH2_CAPTURE_DOWNVAL = TIM_GetCapture2(TIM1);
                TIM_OC2PolarityConfig(TIM1, TIM_ICPolarity_Rising); //CC1P=0 ����Ϊ�����ز���

                if(TIM1CH2_CAPTURE_DOWNVAL < TIM1CH2_CAPTURE_UPVAL)
                {
                    tempup12 = 65536 - (TIM1CH2_CAPTURE_UPVAL - TIM1CH2_CAPTURE_DOWNVAL);
                }
                else
                {
                    tempup12 = TIM1CH2_CAPTURE_DOWNVAL - TIM1CH2_CAPTURE_UPVAL;
                }
								pwmout12=tempup12;//���������õ���FOMD��Ϣ��	
								MyControl.LeftForward.angle_now= 78125/tempup12/2;
							//	printf("SPEED: %f \r\n",MyControl.LeftForward.angle_now);
								if(PEin(3))
									MyTurnflag.LF_TURN =1;
								else
									MyTurnflag.LF_TURN =0;
               // printf("12HIGH:%d us\r\n ", tempup12);
                TIM1CH2_CAPTURE_STA = 0;//������һ�β���						
            }
            else                                //��δ��ʼ,��һ�β���������
            {
                TIM1CH2_CAPTURE_STA = 0;            //���
                TIM1CH2_CAPTURE_UPVAL = TIM_GetCapture2(TIM1);
                TIM1CH2_CAPTURE_STA |= 0X40;        //��ǲ�����������
                TIM_OC2PolarityConfig(TIM1, TIM_ICPolarity_Falling);        //CC1P=1 ����Ϊ�½��ز���							 
            }
        }
    }

    if((TIM1CH3_CAPTURE_STA & 0X80) == 0) //��δ�ɹ�����,���ɼ���Ϣ��ֻ�ж�����
    {
        if(TIM_GetITStatus(TIM1, TIM_IT_CC3) != RESET) //����1���������¼�
        {		
            if(TIM1CH3_CAPTURE_STA & 0X40)      //����һ���½���
            {
                TIM1CH3_CAPTURE_STA |= 0X80;        //��ǳɹ�����һ�θߵ�ƽ����
                TIM1CH3_CAPTURE_DOWNVAL = TIM_GetCapture3(TIM1);
                TIM_OC3PolarityConfig(TIM1, TIM_ICPolarity_Rising); //CC1P=0 ����Ϊ�����ز���

                if(TIM1CH3_CAPTURE_DOWNVAL < TIM1CH3_CAPTURE_UPVAL)
                {
                    tempup13 = 65536 - (TIM1CH3_CAPTURE_UPVAL - TIM1CH3_CAPTURE_DOWNVAL);
                }
                else
                {
                    tempup13 = TIM1CH3_CAPTURE_DOWNVAL - TIM1CH3_CAPTURE_UPVAL;
                }
								pwmout13=tempup13;//�����õ���HOLD��Ϣ��
								MyControl.LeftBehind.angle_now= 78125/tempup13/2;
								//printf("SPEED: %f \r\n",MyControl.LeftBehind.angle_now);
								if(PEin(4))
									MyTurnflag.LB_TURN =1;
								else
									MyTurnflag.LB_TURN =0;
								//printf("%d \r\n",MyTurnflag.LB_TURN);
                //printf("13HIGH:%d us\r\n", tempup13);
                TIM1CH3_CAPTURE_STA = 0;//������һ�β���
            }
            else                                //��δ��ʼ,��һ�β���������
            {
                TIM1CH3_CAPTURE_STA = 0;            //���
                TIM1CH3_CAPTURE_UPVAL = TIM_GetCapture3(TIM1);
                TIM1CH3_CAPTURE_STA |= 0X40;        //��ǲ�����������
                TIM_OC3PolarityConfig(TIM1, TIM_ICPolarity_Falling);        //CC1P=1 ����Ϊ�½��ز���
							
								
            }
        }
    }
    if((TIM1CH4_CAPTURE_STA & 0X80) == 0) //��δ�ɹ�����
    {
        if(TIM_GetITStatus(TIM1, TIM_IT_CC4) != RESET) //����1���������¼�
        {		
            if(TIM1CH4_CAPTURE_STA & 0X40)      //����һ���½���
            {
                TIM1CH4_CAPTURE_STA |= 0X80;        //��ǳɹ�����һ�θߵ�ƽ����
                TIM1CH4_CAPTURE_DOWNVAL = TIM_GetCapture4(TIM1);
                TIM_OC4PolarityConfig(TIM1, TIM_ICPolarity_Rising); //CC1P=0 ����Ϊ�����ز���

                if(TIM1CH4_CAPTURE_DOWNVAL < TIM1CH4_CAPTURE_UPVAL)
                {
                    tempup14 = 65536 - (TIM1CH4_CAPTURE_UPVAL - TIM1CH4_CAPTURE_DOWNVAL);
                }
                else
                {
                    tempup14 = TIM1CH4_CAPTURE_DOWNVAL - TIM1CH4_CAPTURE_UPVAL;
                }
							//	pwmout14=tempup14;
								
								MyControl.RightBehind.angle_now= 78125/tempup14/2;
								//printf("SPEED: %f \r\n",MyControl.RightBehind.angle_now);
								if(PFin(0))
									MyTurnflag.RB_TURN =1;
								else
									MyTurnflag.RB_TURN =0;
								//printf("%d   \r\n",MyTurnflag.RB_TURN);
                //printf("HIGH:%d us ,speed %d\r\n", tempup14,speed1);
								//printf("%d  ",speed1);
                TIM1CH4_CAPTURE_STA = 0;//������һ�β���
								
            }
            else                                //��δ��ʼ,��һ�β���������
            {
                TIM1CH4_CAPTURE_STA = 0;            //���
                TIM1CH4_CAPTURE_UPVAL = TIM_GetCapture4(TIM1);
                TIM1CH4_CAPTURE_STA |= 0X40;        //��ǲ�����������
                TIM_OC4PolarityConfig(TIM1, TIM_ICPolarity_Falling);        //CC1P=1 ����Ϊ�½��ز���
							 //TIM_OC4PolarityConfig(TIM1, TIM_ICPolarity_Rising); //CC1P=0 ����Ϊ�����ز���
								
            }
        }
    }

    TIM_ClearITPendingBit(TIM1, TIM_IT_CC1 | TIM_IT_CC2 | TIM_IT_CC3 | TIM_IT_CC4 | TIM_IT_Update); //����жϱ�־λ
}



void DeHight()//�жϹ���ģʽ�ĺ���������FMOD��HOLD�������໥������4�ֲ�ͬ��ģʽ
{
	 static unsigned char time1=0,time2=0,time3=0,time4=0;  
	
		if((pwmout13>1500)&&(pwmout12<1500))time1++;
		else time1=0;
		if(time1>10){Control_Mode=1;time1=0;}//һ�����ģʽ
		
		
		if((pwmout13>1500)&&(pwmout12>=1500))time2++;
		else time2=0;
		if(time2>10){Control_Mode=2;time2=0;}//һ������ģʽ
		
		if((pwmout13<=1500)&&(pwmout12<1500))time3++;
		else time3=0;
		if(time3>10){Control_Mode=3;time3=0;}//������ͣģʽ
		
		if((pwmout13<=1500)&&(pwmout12>=1500))time4++;
		else time4=0;
		if(time4>10){Control_Mode=0;time4=0;}//��ͨģʽ
		
}



