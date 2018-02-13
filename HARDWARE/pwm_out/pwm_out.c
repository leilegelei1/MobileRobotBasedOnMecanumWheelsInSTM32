/********************************************************************************
 * 文件名  ：pwm_output.c
 * 描述    ：         
 * 实验平台：STM32F103ZET
 * PWM接口 		
 * 库版本  ：3.5.0
 * 作者    ：张磊
 * 时间    ：2015年8月20日10:29:20 
**********************************************************************************/

//注意，本来用的是TIM5的，但是不知道为什么TIM5总是有问题，没办法，改成TIM3试试算了

#include  "pwm_out.h"


///*
// * 函数名：TIM5_GPIO_Config
// * 描述  ：配置TIM5复用输出PWM时用到的I/O
// * 输入  ：无
// * 输出  ：无
// * 调用  ：内部调用
// */
// 
// 
// //PA0,PA1,PA2,PA3 TIM5的四个通道
//static void TIM5_GPIO_Config(void) 
//{
//  GPIO_InitTypeDef GPIO_InitStructure;   //定义引脚声明变量
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);	//使能定时器5时钟
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);  //使能GPIO外设和AFIO复用功能模块时钟
//	
//	
//	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3 ;//声明引脚
//	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;//复用推挽输出
//	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;//定义转换频率50MHz
//	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIO

//}

///*
// * 函数名：TIM5_Mode_Config
// * 描述  ：配置TIM5定时器的工作模式
// * 输入  ：无
// * 输出  ：无
// * 调用  ：内部调用
// */

//static void TIM5_Mode_Config(u16 CCR1_Val,u16 CCR2_Val,u16 CCR3_Val,u16 CCR4_Val)//内部函数，供内部调用
//{
//	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
//	TIM_OCInitTypeDef  TIM_OCInitStructure;
//	
//	TIM_TimeBaseStructure.TIM_Period = 20000-1; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值就是输出的PWM的周期为8MS
//	TIM_TimeBaseStructure.TIM_Prescaler =72-1; //设置用来作为TIMx时钟频率除数的预分频值，就是预分频位720 
//	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
//	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
//	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
//	
//	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	    //配置为PWM模式1
//  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	
//  TIM_OCInitStructure.TIM_Pulse = 0;	   //设置跳变值，当计数器计数到这个值时，电平发生跳变
//  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  //当定时器计数值小于CCR1_Val时为高电平
//  
//	
//	TIM_OC1Init(TIM5, &TIM_OCInitStructure);	 //使能通道1
//  TIM_OC1PreloadConfig(TIM5, TIM_OCPreload_Enable);
//	TIM_OC2Init(TIM5, &TIM_OCInitStructure);	 //使能通道2
//  TIM_OC2PreloadConfig(TIM5, TIM_OCPreload_Enable);
//  TIM_OC3Init(TIM5, &TIM_OCInitStructure);	 //使能通道3
//  TIM_OC3PreloadConfig(TIM5, TIM_OCPreload_Enable);
//  TIM_OC4Init(TIM5, &TIM_OCInitStructure);	 //使能通道4
//  TIM_OC4PreloadConfig(TIM5, TIM_OCPreload_Enable);
//	
//	TIM_ARRPreloadConfig(TIM5, ENABLE);			 // 使能TIM5重载寄存器ARR
//	
//  TIM_Cmd(TIM5, ENABLE);                   //使能定时器5
//}

/*
 * 函数名：TIM5_Mode_Config
 * 描述  ：TIM5 输出PWM信号初始化，只要调用这个函数
 *         TIM5的四个通道就会有PWM信号输出
 * 输入  ：无
 * 输出  ：无
 * 调用  ：外部调用
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
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC  | RCC_APB2Periph_AFIO, ENABLE);  //使能GPIO外设和AFIO复用功能模块时钟使能
	
//	GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE); //Timer3部分重映射  TIM3_CH2->PB5                                                                       	 //用于TIM3的CH2输出的PWM通过该LED显示
// 
//   //设置该引脚为复用输出功能,输出TIM3 CH2的PWM脉冲波形
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1; //TIM3_CH3+TIM3_CH4
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOB, &GPIO_InitStructure);
//	//GPIO_WriteBit(GPIOA, GPIO_Pin_7,Bit_SET); // PA7上拉	
//	
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5; //TIM3_CH1+TIM3_CH2
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_PinRemapConfig(GPIO_FullRemap_TIM3, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9; //TIM3_CH3+TIM3_CH4
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_SetBits(GPIOC, GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9); // PA7上拉

	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 80K
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值  不分频
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
	
	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_Pulse = 0; //设置待装入捕获比较寄存器的脉冲值
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
	
	TIM_OC1Init(TIM3, &TIM_OCInitStructure);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);  //使能TIMx在CCR2上的预装载寄存器
	
	TIM_OC2Init(TIM3, &TIM_OCInitStructure);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);  //使能TIMx在CCR2上的预装载寄存器
	
	TIM_OC3Init(TIM3, &TIM_OCInitStructure);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx
	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);  //使能TIMx在CCR2上的预装载寄存器
	
	TIM_OC4Init(TIM3, &TIM_OCInitStructure);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx
	TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);  //使能TIMx在CCR2上的预装载寄存器
	
	TIM_ARRPreloadConfig(TIM3, ENABLE); //使能TIMx在ARR上的预装载寄存器
	
 
	TIM_Cmd(TIM3, ENABLE);  //使能TIMx外设
 

}
/*
 * 函数名：TIM5_PWM_OUTPUT
 * 描述  ：TIM5输出可控PWM信号
 * 输入  ：DR1
 * 输出  ：无
 * 调用	 ：外部调用		   
 */

void TIM3_PWM_OUTPUT(u16 DR1,u16 DR2,u16 DR3,u16 DR4)
{
	TIM_SetCompare1(TIM3,DR1);
	TIM_SetCompare2(TIM3,DR2);
	TIM_SetCompare3(TIM3,DR3);
	TIM_SetCompare4(TIM3,DR4);
}



//定时器2也用作于输出吧
void TIM2_PWM_Init(u16 arr,u16 psc)//
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA  | RCC_APB2Periph_AFIO, ENABLE);  //使能GPIO外设和AFIO复用功能模块时钟使能
	
//	GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE); //Timer3部分重映射  TIM3_CH2->PB5                                                                       	 //用于TIM3的CH2输出的PWM通过该LED显示
 
   //设置该引脚为复用输出功能,输出TIM3 CH2的PWM脉冲波形
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3; //TIM3_CH3+TIM3_CH4
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//GPIO_WriteBit(GPIOA, GPIO_Pin_7,Bit_SET); // PA7上拉	
	

	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 80K
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值  不分频
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
	
	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_Pulse = 0; //设置待装入捕获比较寄存器的脉冲值
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
	
	TIM_OC1Init(TIM2, &TIM_OCInitStructure);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx
	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);  //使能TIMx在CCR2上的预装载寄存器
	
	TIM_OC2Init(TIM2, &TIM_OCInitStructure);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx
	TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);  //使能TIMx在CCR2上的预装载寄存器
	
	TIM_OC3Init(TIM2, &TIM_OCInitStructure);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx
	TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);  //使能TIMx在CCR2上的预装载寄存器
	
	TIM_OC4Init(TIM2, &TIM_OCInitStructure);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx
	TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);  //使能TIMx在CCR2上的预装载寄存器
	
	TIM_ARRPreloadConfig(TIM2, ENABLE); //使能TIMx在ARR上的预装载寄存器
	
 
	TIM_Cmd(TIM2, ENABLE);  //使能TIMx外设
 
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







////下面的这两个函数是用来控制云台的，暂时还用不到，，，，本来是应该用TIM4的但是因为TIM4是支持TF的被临时征用去做接收机了


////只能用TIM5了
//static void TIM5_GPIO_Config(void)
//{
//	
//  GPIO_InitTypeDef GPIO_InitStructure;   //定义引脚声明变量
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);	//使能定时器4时钟
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);  //使能GPIO外设和AFIO复用功能模块时钟
//	
//	
//	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;//声明引脚
//	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;//复用推挽输出
//	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;//定义转换频率50MHz
//	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIO

//}



//static void TIM5_Mode_Config(void)
//{
//	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
//	TIM_OCInitTypeDef  TIM_OCInitStructure;

//  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);  
//	
//	/* Time base configuration */		 
//  TIM_TimeBaseStructure.TIM_Period = 25000-1;      //PWM周期
//  TIM_TimeBaseStructure.TIM_Prescaler = 72-1;	    //设置预分频：即为1MHz
//  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;	//设置时钟分频系数：不分频
//  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //向上计数模式

//  TIM_TimeBaseStructure.TIM_Period = 20000-1;  //PWM周期

//  TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);
//  TIM_ARRPreloadConfig(TIM5, ENABLE);

//  /* PWM1 Mode configuration: Channel1 */
//  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	    //配置为PWM模式1
//  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	
//  TIM_OCInitStructure.TIM_Pulse = 1500;	   //设置跳变值，当计数器计数到这个值时，电平发生跳变
//  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  //当定时器计数值小于CCR1_Val时为高电平

//	 TIM_OC1Init(TIM5, &TIM_OCInitStructure);	 //使能通道3
//  TIM_OC1PreloadConfig(TIM5, TIM_OCPreload_Enable);
//	 TIM_OC2Init(TIM5, &TIM_OCInitStructure);	 //使能通道3
//  TIM_OC2PreloadConfig(TIM5, TIM_OCPreload_Enable);

//  TIM_OC3Init(TIM5, &TIM_OCInitStructure);	 //使能通道3
//  TIM_OC3PreloadConfig(TIM5, TIM_OCPreload_Enable);
//  TIM_OC4Init(TIM5, &TIM_OCInitStructure);	 //使能通道4
//  TIM_OC4PreloadConfig(TIM5, TIM_OCPreload_Enable);

//  TIM_ARRPreloadConfig(TIM5, ENABLE);			 // 使能TIM5重载寄存器ARR
//	
//  TIM_Cmd(TIM5, ENABLE);                   //使能定时器4
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

