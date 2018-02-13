/********************************************************************************
* 文件名  ：pwm_output.c
* 描述    ：
* 实验平台：STM32F103ZET
* PWM接口
* 库版本  ：3.5.0
* 作者    ：张磊
* 时间    ：2015年8月21日23:26:26
**********************************************************************************/


#include "pwm_in.h"
#include "sys.h"
#include "usart.h"
#include "pid.h"

//这个源文件目前还用不到是以后用的，这是接受遥控器所传来数据的，等以后配了遥控器再说吧

//这个文件必须得注意，TIM5是不能容忍直接输入5V的 但是TIM4却支持，所以一定要使用TIM4作为接受信号的定时器，不然就会烧坏芯片

u8  TIM4CH1_CAPTURE_STA = 0;    //通道1输入捕获状态       用高两位做捕获标志，低六位做溢出计数
int TIM4CH1_CAPTURE_UPVAL;  //通道1输入捕获值
int TIM4CH1_CAPTURE_DOWNVAL;    //通道1输入捕获值

u8  TIM4CH2_CAPTURE_STA = 0;    //通道2输入捕获状态
int TIM4CH2_CAPTURE_UPVAL;  //通道2输入捕获值
int TIM4CH2_CAPTURE_DOWNVAL;    //通道2输入捕获值

u8  TIM4CH3_CAPTURE_STA = 0;    //通道3输入捕获状态
int TIM4CH3_CAPTURE_UPVAL;  //通道3输入捕获值
int TIM4CH3_CAPTURE_DOWNVAL;    //通道3输入捕获值

u8  TIM4CH4_CAPTURE_STA = 0;    //通道4输入捕获状态
int TIM4CH4_CAPTURE_UPVAL;  //通道4输入捕获值
int TIM4CH4_CAPTURE_DOWNVAL;    //通道4输入捕获值


u8 SPEED_ROLL_FLAG=1;
TURNFLAG MyTurnflag ={1,1,1,1};//转向的标志位


int tempup1 = 0;//捕获总高电平的时间us单位
int tempup2 = 0;//捕获总高电平的时间us单位
int tempup3 = 0;//捕获总高电平的时间us单位
int tempup4 = 0;//捕获总高电平的时间us单位
u32 tim3_T;

int pwmout1, pwmout2, pwmout3, pwmout4;                 //输出占空比


int speed1;

unsigned char Control_Mode;



//再加上一个TIM1的捕获输入，用来得到其他的数值量

u8  TIM1CH1_CAPTURE_STA = 0;    //通道1输入捕获状态       用高两位做捕获标志，低六位做溢出计数
int TIM1CH1_CAPTURE_UPVAL;  //通道1输入捕获值
int TIM1CH1_CAPTURE_DOWNVAL;    //通道1输入捕获值

u8  TIM1CH2_CAPTURE_STA = 0;    //通道2输入捕获状态
int TIM1CH2_CAPTURE_UPVAL;  //通道2输入捕获值
int TIM1CH2_CAPTURE_DOWNVAL;    //通道2输入捕获值

u8  TIM1CH3_CAPTURE_STA = 0;    //通道3输入捕获状态
int TIM1CH3_CAPTURE_UPVAL;  //通道3输入捕获值
int TIM1CH3_CAPTURE_DOWNVAL;    //通道3输入捕获值

u8  TIM1CH4_CAPTURE_STA = 0;    //通道4输入捕获状态
int TIM1CH4_CAPTURE_UPVAL;  //通道4输入捕获值
int TIM1CH4_CAPTURE_DOWNVAL;    //通道4输入捕获值

u8 whatup;


int tempup11 = 0;//捕获总高电平的时间us单位
int tempup12 = 0;//捕获总高电平的时间us单位
int tempup13 = 0;//捕获总高电平的时间us单位
int tempup14 = 0;//捕获总高电平的时间us单位

int pwmout11, pwmout12, pwmout13, pwmout14;                 //输出占空比


//TIM4定时器匹配，用于检测遥控四通道的输入信号

void pwm_in_init(void)//定时器4，用于接收遥控器的信号
{
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_ICInitTypeDef  TIM4_ICInitStructure;
    //  EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);    //使能定时器4时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);  //使能GPIO外设和AFIO复用功能模块时钟，启动重映射，放到D端口
	
		GPIO_PinRemapConfig(GPIO_Remap_TIM4, ENABLE); //开启重映射，因为前面的时候和蜂鸣器有冲突很吵
	
//    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;  //PA0 清除之前设置
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; // 输入
//    GPIO_Init(GPIOB, &GPIO_InitStructure);
//    GPIO_ResetBits(GPIOB, GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9);                        // 下拉
	
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;  //PA0 清除之前设置
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; // 输入
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    GPIO_ResetBits(GPIOD, GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);   
	
	
    //  GPIO_InitStructure1.GPIO_Pin  = GPIO_Pin_0|GPIO_Pin_1;  //PA0 清除之前设置
    //  GPIO_InitStructure1.GPIO_Mode = GPIO_Mode_IPD; //PA0 输入
    //  GPIO_Init(GPIOB, &GPIO_InitStructure1);
    //  GPIO_ResetBits(GPIOB,GPIO_Pin_0|GPIO_Pin_1);                         //PA0 下拉
    //--------------------------------------------------配置定时器5输入捕获--------------------------------------------
	
	
	
    //初始化定时器4 TIM4
    TIM_TimeBaseStructure.TIM_Period = 0XFFFF; //设定计数器自动重装值
    TIM_TimeBaseStructure.TIM_Prescaler = 72 - 1;   //预分频器
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
    //初始化TIM3输入捕获参数
    TIM4_ICInitStructure.TIM_Channel = TIM_Channel_1; //CC1S=01     选择输入端 IC1映射到TI1上
    TIM4_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;    //上升沿捕获
    TIM4_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //映射到TI1上
    TIM4_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;   //配置输入分频,不分频
    TIM4_ICInitStructure.TIM_ICFilter = 0x00;//IC1F=0000 配置输入滤波器 不滤波
    TIM_ICInit(TIM4, &TIM4_ICInitStructure);
    TIM4_ICInitStructure.TIM_Channel = TIM_Channel_2; //CC1S=01     选择输入端 IC1映射到TI1上
    TIM4_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;    //上升沿捕获
    TIM4_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //映射到TI1上
    TIM4_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;   //配置输入分频,不分频
    TIM4_ICInitStructure.TIM_ICFilter = 0x00;//IC1F=0000 配置输入滤波器 不滤波
    TIM_ICInit(TIM4, &TIM4_ICInitStructure);
    TIM4_ICInitStructure.TIM_Channel = TIM_Channel_3; //CC1S=01     选择输入端 IC1映射到TI1上
    TIM4_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;    //上升沿捕获
    TIM4_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //映射到TI1上
    TIM4_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;   //配置输入分频,不分频
    TIM4_ICInitStructure.TIM_ICFilter = 0x00;//IC1F=0000 配置输入滤波器 不滤波
    TIM_ICInit(TIM4, &TIM4_ICInitStructure);
    TIM4_ICInitStructure.TIM_Channel = TIM_Channel_4; //CC1S=01     选择输入端 IC1映射到TI1上
    TIM4_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;    //上升沿捕获
    TIM4_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //映射到TI1上
    TIM4_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;   //配置输入分频,不分频
    TIM4_ICInitStructure.TIM_ICFilter = 0x00;//IC1F=0000 配置输入滤波器 不滤波
    TIM_ICInit(TIM4, &TIM4_ICInitStructure);
    //中断分组初始化
    NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  //TIM5中断
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //先占优先级2级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  //从优先级1级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
    NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
    TIM_ITConfig(TIM4, TIM_IT_Update | TIM_IT_CC1, ENABLE);//允许更新中断 ,允许CC1IE捕获中断
		
		
    TIM_ITConfig(TIM4, TIM_IT_CC1, ENABLE);//允许更新中断 ,允许CC1IE捕获中断
    TIM_ITConfig(TIM4, TIM_IT_CC2, ENABLE);//允许更新中断 ,允许CC1IE捕获中断
    TIM_ITConfig(TIM4, TIM_IT_CC3, ENABLE);//允许更新中断 ,允许CC1IE捕获中断
    TIM_ITConfig(TIM4, TIM_IT_CC4, ENABLE);//允许更新中断 ,允许CC1IE捕获中断
		
    TIM_Cmd(TIM4, ENABLE);  //使能定时器4
}





//定时器4中断服务程序
void TIM4_IRQHandler(void)
{
    if((TIM4CH1_CAPTURE_STA & 0X80) == 0) //还未成功捕获
    {
        if(TIM_GetITStatus(TIM4, TIM_IT_CC1) != RESET) //捕获1发生捕获事件
        {
            if(TIM4CH1_CAPTURE_STA & 0X40)      //捕获到一个下降沿
            {
                TIM4CH1_CAPTURE_STA |= 0X80;        //标记成功捕获到一次高电平脉宽
                TIM4CH1_CAPTURE_DOWNVAL = TIM_GetCapture1(TIM4);
                TIM_OC1PolarityConfig(TIM4, TIM_ICPolarity_Rising); //CC1P=0 设置为上升沿捕获

                if(TIM4CH1_CAPTURE_DOWNVAL < TIM4CH1_CAPTURE_UPVAL)
                {
                    tempup1 = 65536 - (TIM4CH1_CAPTURE_UPVAL - TIM4CH1_CAPTURE_DOWNVAL);
                }
                else
                {
                    tempup1 = TIM4CH1_CAPTURE_DOWNVAL - TIM4CH1_CAPTURE_UPVAL;
                }
								pwmout1=tempup1;
                //printf("1  HIGH:%d us\r\n", tempup1);//输出高电平时间
                TIM4CH1_CAPTURE_STA = 0;//开启下一次捕获
            }
            else                                //还未开始,第一次捕获上升沿
            {
                TIM4CH1_CAPTURE_STA = 0;            //清空
                TIM4CH1_CAPTURE_UPVAL = TIM_GetCapture1(TIM4);
                TIM4CH1_CAPTURE_STA |= 0X40;        //标记捕获到了上升沿
                TIM_OC1PolarityConfig(TIM4, TIM_ICPolarity_Falling);        //CC1P=1 设置为下降沿捕获
            }
        }
    }

    if((TIM4CH2_CAPTURE_STA & 0X80) == 0) //还未成功捕获
    {
        if(TIM_GetITStatus(TIM4, TIM_IT_CC2) != RESET) //捕获1发生捕获事件
        {		
            if(TIM4CH2_CAPTURE_STA & 0X40)      //捕获到一个下降沿
            {
                TIM4CH2_CAPTURE_STA |= 0X80;        //标记成功捕获到一次高电平脉宽
                TIM4CH2_CAPTURE_DOWNVAL = TIM_GetCapture2(TIM4);
                TIM_OC2PolarityConfig(TIM4, TIM_ICPolarity_Rising); //CC1P=0 设置为上升沿捕获

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
                TIM4CH2_CAPTURE_STA = 0;//开启下一次捕获
            }
            else                                //还未开始,第一次捕获上升沿
            {
                TIM4CH2_CAPTURE_STA = 0;            //清空
                TIM4CH2_CAPTURE_UPVAL = TIM_GetCapture2(TIM4);
                TIM4CH2_CAPTURE_STA |= 0X40;        //标记捕获到了上升沿
                TIM_OC2PolarityConfig(TIM4, TIM_ICPolarity_Falling);        //CC1P=1 设置为下降沿捕获
            }
        }
    }

    if((TIM4CH3_CAPTURE_STA & 0X80) == 0) //还未成功捕获
    {
        if(TIM_GetITStatus(TIM4, TIM_IT_CC3) != RESET) //捕获1发生捕获事件
        {		
            if(TIM4CH3_CAPTURE_STA & 0X40)      //捕获到一个下降沿
            {
                TIM4CH3_CAPTURE_STA |= 0X80;        //标记成功捕获到一次高电平脉宽
                TIM4CH3_CAPTURE_DOWNVAL = TIM_GetCapture3(TIM4);
                TIM_OC3PolarityConfig(TIM4, TIM_ICPolarity_Rising); //CC1P=0 设置为上升沿捕获

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
                TIM4CH3_CAPTURE_STA = 0;//开启下一次捕获
            }
            else                                //还未开始,第一次捕获上升沿
            {
                TIM4CH3_CAPTURE_STA = 0;            //清空
                TIM4CH3_CAPTURE_UPVAL = TIM_GetCapture3(TIM4);
                TIM4CH3_CAPTURE_STA |= 0X40;        //标记捕获到了上升沿
                TIM_OC3PolarityConfig(TIM4, TIM_ICPolarity_Falling);        //CC1P=1 设置为下降沿捕获
            }
        }
    }
    if((TIM4CH4_CAPTURE_STA & 0X80) == 0) //还未成功捕获
    {
        if(TIM_GetITStatus(TIM4, TIM_IT_CC4) != RESET) //捕获1发生捕获事件
        {		
            if(TIM4CH4_CAPTURE_STA & 0X40)      //捕获到一个下降沿
            {
                TIM4CH4_CAPTURE_STA |= 0X80;        //标记成功捕获到一次高电平脉宽
                TIM4CH4_CAPTURE_DOWNVAL = TIM_GetCapture4(TIM4);
                TIM_OC4PolarityConfig(TIM4, TIM_ICPolarity_Rising); //CC1P=0 设置为上升沿捕获

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
                TIM4CH4_CAPTURE_STA = 0;//开启下一次捕获
            }
            else                                //还未开始,第一次捕获上升沿
            {
                TIM4CH4_CAPTURE_STA = 0;            //清空
                TIM4CH4_CAPTURE_UPVAL = TIM_GetCapture4(TIM4);
                TIM4CH4_CAPTURE_STA |= 0X40;        //标记捕获到了上升沿
                TIM_OC4PolarityConfig(TIM4, TIM_ICPolarity_Falling);        //CC1P=1 设置为下降沿捕获
            }
        }
    }

    TIM_ClearITPendingBit(TIM4, TIM_IT_CC1 | TIM_IT_CC2 | TIM_IT_CC3 | TIM_IT_CC4 | TIM_IT_Update); //清除中断标志位
}






void PIN2_Init()//初始化引脚，用于测量电机的转向
{
	GPIO_InitTypeDef  GPIO_InitStructure;
 	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE|RCC_APB2Periph_GPIOF, ENABLE);	 //使能PB,PE端口时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF|RCC_APB2Periph_GPIOF, ENABLE);	 //使能PB,PE端口时钟
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4;//|GPIO_Pin_6;				 //LED0-->PB.5 端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 		 //推挽输出
	//GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	GPIO_Init(GPIOE, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB.5
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;				 //LED0-->PB.5 端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 		 //推挽输出
	//GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	GPIO_Init(GPIOF, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB.5
	
	//GPIO_SetBits(GPIOE,GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5);						 //PB.5 输出高
	
}



void tim1_pwm_in_init(void)//TIM1输入捕获，用于测量电机速度
{
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_ICInitTypeDef  TIM1_ICInitStructure;
    //  EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);    //使能定时器1时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE | RCC_APB2Periph_AFIO, ENABLE);  //使能GPIO外设和AFIO复用功能模块时钟，启动重映射，放到D端口
	
		GPIO_PinRemapConfig(GPIO_FullRemap_TIM1, ENABLE); 
	
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_11 | GPIO_Pin_13 | GPIO_Pin_14;  //PA0 清除之前设置
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; // 输入
    GPIO_Init(GPIOE, &GPIO_InitStructure);//这里需要注意TIM1启动重映射以后的对应端口实在PE端口的不要搞错了
    GPIO_ResetBits(GPIOE, GPIO_Pin_9 | GPIO_Pin_11 | GPIO_Pin_13 | GPIO_Pin_14);                        // 下拉
	
	
	
	
    //  GPIO_InitStructure1.GPIO_Pin  = GPIO_Pin_0|GPIO_Pin_1;  //PA0 清除之前设置
    //  GPIO_InitStructure1.GPIO_Mode = GPIO_Mode_IPD; //PA0 输入
    //  GPIO_Init(GPIOB, &GPIO_InitStructure1);
    //  GPIO_ResetBits(GPIOB,GPIO_Pin_0|GPIO_Pin_1);                         //PA0 下拉
    //--------------------------------------------------配置定时器5输入捕获--------------------------------------------
	
	
	
    //初始化定时器1 TIM1
    TIM_TimeBaseStructure.TIM_Period = 0XFFFF; //设定计数器自动重装值
    TIM_TimeBaseStructure.TIM_Prescaler = 72 - 1;   //预分频器
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
    //初始化TIM3输入捕获参数
    TIM1_ICInitStructure.TIM_Channel = TIM_Channel_1; //CC1S=01     选择输入端 IC1映射到TI1上
    TIM1_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;    //上升沿捕获
    TIM1_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //映射到TI1上
    TIM1_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;   //配置输入分频,不分频
    TIM1_ICInitStructure.TIM_ICFilter = 0x00;//IC1F=0000 配置输入滤波器 不滤波
    TIM_ICInit(TIM1, &TIM1_ICInitStructure);
    TIM1_ICInitStructure.TIM_Channel = TIM_Channel_2; //CC1S=01     选择输入端 IC1映射到TI1上
    TIM1_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;    //上升沿捕获
    TIM1_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //映射到TI1上
    TIM1_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;   //配置输入分频,不分频
    TIM1_ICInitStructure.TIM_ICFilter = 0x00;//IC1F=0000 配置输入滤波器 不滤波
    TIM_ICInit(TIM1, &TIM1_ICInitStructure);
    TIM1_ICInitStructure.TIM_Channel = TIM_Channel_3; //CC1S=01     选择输入端 IC1映射到TI1上
    TIM1_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;    //上升沿捕获
    TIM1_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //映射到TI1上
    TIM1_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;   //配置输入分频,不分频
    TIM1_ICInitStructure.TIM_ICFilter = 0x00;//IC1F=0000 配置输入滤波器 不滤波
    TIM_ICInit(TIM1, &TIM1_ICInitStructure);
    TIM1_ICInitStructure.TIM_Channel = TIM_Channel_4; //CC1S=01     选择输入端 IC1映射到TI1上
    TIM1_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;    //上升沿捕获
    TIM1_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //映射到TI1上
    TIM1_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;   //配置输入分频,不分频
    TIM1_ICInitStructure.TIM_ICFilter = 0x00;//IC1F=0000 配置输入滤波器 不滤波
    TIM_ICInit(TIM1, &TIM1_ICInitStructure);
    //中断分组初始化
    NVIC_InitStructure.NVIC_IRQChannel = TIM1_CC_IRQn;  //TIM1捕获中断
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;  //先占优先级2级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级0级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
    NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
    TIM_ITConfig(TIM1, TIM_IT_Update | TIM_IT_CC1, ENABLE);//允许更新中断 ,允许CC1IE捕获中断
		
		
    TIM_ITConfig(TIM1, TIM_IT_CC1, ENABLE);//允许更新中断 ,允许CC1IE捕获中断
    TIM_ITConfig(TIM1, TIM_IT_CC2, ENABLE);//允许更新中断 ,允许CC1IE捕获中断
    TIM_ITConfig(TIM1, TIM_IT_CC3, ENABLE);//允许更新中断 ,允许CC1IE捕获中断
    TIM_ITConfig(TIM1, TIM_IT_CC4, ENABLE);//允许更新中断 ,允许CC1IE捕获中断
		
    TIM_Cmd(TIM1, ENABLE);  //使能定时器4
}


//定时器1中断服务程序
void TIM1_CC_IRQHandler(void)
{
    if((TIM1CH1_CAPTURE_STA & 0X80) == 0) //还未成功捕获
    {
        if(TIM_GetITStatus(TIM1, TIM_IT_CC1) != RESET) //捕获1发生捕获事件
        {
            if(TIM1CH1_CAPTURE_STA & 0X40)      //捕获到一个下降沿
            {
                TIM1CH1_CAPTURE_STA |= 0X80;        //标记成功捕获到一次高电平脉宽
                TIM1CH1_CAPTURE_DOWNVAL = TIM_GetCapture1(TIM1);
                TIM_OC1PolarityConfig(TIM1, TIM_ICPolarity_Rising); //CC1P=0 设置为上升沿捕获

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
                TIM1CH1_CAPTURE_STA = 0;//开启下一次捕获
            }
            else                                //还未开始,第一次捕获上升沿
            {
                TIM1CH1_CAPTURE_STA = 0;            //清空
                TIM1CH1_CAPTURE_UPVAL = TIM_GetCapture1(TIM1);
                TIM1CH1_CAPTURE_STA |= 0X40;        //标记捕获到了上升沿
                TIM_OC1PolarityConfig(TIM1, TIM_ICPolarity_Falling);        //CC1P=1 设置为下降沿捕获
            }
        }
    }

    if((TIM1CH2_CAPTURE_STA & 0X80) == 0) //还未成功捕获
    {
        if(TIM_GetITStatus(TIM1, TIM_IT_CC2) != RESET) //捕获1发生捕获事件
        {		
            if(TIM1CH2_CAPTURE_STA & 0X40)      //捕获到一个下降沿
            {
                TIM1CH2_CAPTURE_STA |= 0X80;        //标记成功捕获到一次高电平脉宽
                TIM1CH2_CAPTURE_DOWNVAL = TIM_GetCapture2(TIM1);
                TIM_OC2PolarityConfig(TIM1, TIM_ICPolarity_Rising); //CC1P=0 设置为上升沿捕获

                if(TIM1CH2_CAPTURE_DOWNVAL < TIM1CH2_CAPTURE_UPVAL)
                {
                    tempup12 = 65536 - (TIM1CH2_CAPTURE_UPVAL - TIM1CH2_CAPTURE_DOWNVAL);
                }
                else
                {
                    tempup12 = TIM1CH2_CAPTURE_DOWNVAL - TIM1CH2_CAPTURE_UPVAL;
                }
								pwmout12=tempup12;//这个函数获得的是FOMD信息量	
								MyControl.LeftForward.angle_now= 78125/tempup12/2;
							//	printf("SPEED: %f \r\n",MyControl.LeftForward.angle_now);
								if(PEin(3))
									MyTurnflag.LF_TURN =1;
								else
									MyTurnflag.LF_TURN =0;
               // printf("12HIGH:%d us\r\n ", tempup12);
                TIM1CH2_CAPTURE_STA = 0;//开启下一次捕获						
            }
            else                                //还未开始,第一次捕获上升沿
            {
                TIM1CH2_CAPTURE_STA = 0;            //清空
                TIM1CH2_CAPTURE_UPVAL = TIM_GetCapture2(TIM1);
                TIM1CH2_CAPTURE_STA |= 0X40;        //标记捕获到了上升沿
                TIM_OC2PolarityConfig(TIM1, TIM_ICPolarity_Falling);        //CC1P=1 设置为下降沿捕获							 
            }
        }
    }

    if((TIM1CH3_CAPTURE_STA & 0X80) == 0) //还未成功捕获,不采集信息，只判断正负
    {
        if(TIM_GetITStatus(TIM1, TIM_IT_CC3) != RESET) //捕获1发生捕获事件
        {		
            if(TIM1CH3_CAPTURE_STA & 0X40)      //捕获到一个下降沿
            {
                TIM1CH3_CAPTURE_STA |= 0X80;        //标记成功捕获到一次高电平脉宽
                TIM1CH3_CAPTURE_DOWNVAL = TIM_GetCapture3(TIM1);
                TIM_OC3PolarityConfig(TIM1, TIM_ICPolarity_Rising); //CC1P=0 设置为上升沿捕获

                if(TIM1CH3_CAPTURE_DOWNVAL < TIM1CH3_CAPTURE_UPVAL)
                {
                    tempup13 = 65536 - (TIM1CH3_CAPTURE_UPVAL - TIM1CH3_CAPTURE_DOWNVAL);
                }
                else
                {
                    tempup13 = TIM1CH3_CAPTURE_DOWNVAL - TIM1CH3_CAPTURE_UPVAL;
                }
								pwmout13=tempup13;//这个获得的是HOLD信息量
								MyControl.LeftBehind.angle_now= 78125/tempup13/2;
								//printf("SPEED: %f \r\n",MyControl.LeftBehind.angle_now);
								if(PEin(4))
									MyTurnflag.LB_TURN =1;
								else
									MyTurnflag.LB_TURN =0;
								//printf("%d \r\n",MyTurnflag.LB_TURN);
                //printf("13HIGH:%d us\r\n", tempup13);
                TIM1CH3_CAPTURE_STA = 0;//开启下一次捕获
            }
            else                                //还未开始,第一次捕获上升沿
            {
                TIM1CH3_CAPTURE_STA = 0;            //清空
                TIM1CH3_CAPTURE_UPVAL = TIM_GetCapture3(TIM1);
                TIM1CH3_CAPTURE_STA |= 0X40;        //标记捕获到了上升沿
                TIM_OC3PolarityConfig(TIM1, TIM_ICPolarity_Falling);        //CC1P=1 设置为下降沿捕获
							
								
            }
        }
    }
    if((TIM1CH4_CAPTURE_STA & 0X80) == 0) //还未成功捕获
    {
        if(TIM_GetITStatus(TIM1, TIM_IT_CC4) != RESET) //捕获1发生捕获事件
        {		
            if(TIM1CH4_CAPTURE_STA & 0X40)      //捕获到一个下降沿
            {
                TIM1CH4_CAPTURE_STA |= 0X80;        //标记成功捕获到一次高电平脉宽
                TIM1CH4_CAPTURE_DOWNVAL = TIM_GetCapture4(TIM1);
                TIM_OC4PolarityConfig(TIM1, TIM_ICPolarity_Rising); //CC1P=0 设置为上升沿捕获

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
                TIM1CH4_CAPTURE_STA = 0;//开启下一次捕获
								
            }
            else                                //还未开始,第一次捕获上升沿
            {
                TIM1CH4_CAPTURE_STA = 0;            //清空
                TIM1CH4_CAPTURE_UPVAL = TIM_GetCapture4(TIM1);
                TIM1CH4_CAPTURE_STA |= 0X40;        //标记捕获到了上升沿
                TIM_OC4PolarityConfig(TIM1, TIM_ICPolarity_Falling);        //CC1P=1 设置为下降沿捕获
							 //TIM_OC4PolarityConfig(TIM1, TIM_ICPolarity_Rising); //CC1P=0 设置为上升沿捕获
								
            }
        }
    }

    TIM_ClearITPendingBit(TIM1, TIM_IT_CC1 | TIM_IT_CC2 | TIM_IT_CC3 | TIM_IT_CC4 | TIM_IT_Update); //清除中断标志位
}



void DeHight()//判断工作模式的函数，利用FMOD和HOLD两个的相互配合组成4种不同的模式
{
	 static unsigned char time1=0,time2=0,time3=0,time4=0;  
	
		if((pwmout13>1500)&&(pwmout12<1500))time1++;
		else time1=0;
		if(time1>10){Control_Mode=1;time1=0;}//一键起飞模式
		
		
		if((pwmout13>1500)&&(pwmout12>=1500))time2++;
		else time2=0;
		if(time2>10){Control_Mode=2;time2=0;}//一键降落模式
		
		if((pwmout13<=1500)&&(pwmout12<1500))time3++;
		else time3=0;
		if(time3>10){Control_Mode=3;time3=0;}//定高悬停模式
		
		if((pwmout13<=1500)&&(pwmout12>=1500))time4++;
		else time4=0;
		if(time4>10){Control_Mode=0;time4=0;}//普通模式
		
}



