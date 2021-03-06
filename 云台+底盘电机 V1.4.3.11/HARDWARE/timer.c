#include "timer.h"

void TIM5_Int_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);  ///使能TIM4时钟
	
  TIM_TimeBaseInitStructure.TIM_Period = arr; 	//自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM5,&TIM_TimeBaseInitStructure);//初始化TIM5
	
	TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE); //允许定时器4更新中断
	TIM_Cmd(TIM5,ENABLE); //使能定时器4
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM5_IRQn; //定时器4中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; //抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; //子优先级1
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);	
}
/*************************************
* 	使用TIM14及TIM7
* 	TIM14用于行走pid
*	TIM7用于枪pid
*
*************************************/
void Move_PID_TIM_Init(u16 arr,u16 psc)
{
	//TIM14 用于PID
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
 	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14,ENABLE);  ///使能TIM14时钟
	
  TIM_TimeBaseInitStructure.TIM_Period = arr; 	//自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM14,&TIM_TimeBaseInitStructure);//初始化TIM14
	
	TIM_ITConfig(TIM14,TIM_IT_Update,ENABLE); //允许定时器14更新中断
	TIM_Cmd(TIM14,ENABLE); //使能定时�1�
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM8_TRG_COM_TIM14_IRQn; //定时器14中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x01; //子优先级1
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
}
void Gun_PID_TIM_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
		
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7,ENABLE);  ///使能TIM7时钟
	
  TIM_TimeBaseInitStructure.TIM_Period = arr; 	//自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler = psc;  //定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM7,&TIM_TimeBaseInitStructure);//初始化TIM7
	
	TIM_ITConfig(TIM7,TIM_IT_Update,ENABLE); //允许定时器14更新中断
	TIM_Cmd(TIM7,ENABLE); //使能定时�1�
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM7_IRQn ; //定时器14中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x02; //子优先级1
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
}
void ReceiveBall_TIM_Init(int PWM)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;  
	TIM_ICInitTypeDef TIM_ICInitStructure;  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM10,ENABLE);  		//TIM10时钟使能    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); 	//使能PORTB时钟	
	
	//初始化B10
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource8,GPIO_AF_TIM10); 	//GPIOB8复用为定时器10	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        		//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;			//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      		//推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        		//上拉
	GPIO_Init(GPIOB,&GPIO_InitStructure);               		//初始化PA2/3
	
	TIM_TimeBaseStructure.TIM_Prescaler=84-1;  									//定时器分频
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 	//向上计数模式
	TIM_TimeBaseStructure.TIM_Period=1000-1;   									//自动重装载值 2.0ms
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM10,&TIM_TimeBaseStructure);							//初始化定时器10
	
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 							//选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 	//比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 			//输出极性:TIM输出比较极性高
	
	//初始化TIM10 Channel3 PWM模式	 
	TIM_OC1Init(TIM10, &TIM_OCInitStructure);  					//根据T指定的参数初始化外设TIM10OC1
	TIM_OC1PreloadConfig(TIM10, TIM_OCPreload_Enable);  	//使能TIM10在CCR1上的预装载寄存器
 
   TIM_ARRPreloadConfig(TIM10,ENABLE);	//ARPE使能 
	TIM_Cmd(TIM10, ENABLE);  						//使能TIM10
	TIM10->CCR1 = PWM; //收球
	
}
void SelectBall_TIM_Init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;  
	TIM_ICInitTypeDef TIM_ICInitStructure;  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM11,ENABLE);  		//TIM11时钟使能    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); 	//使能PORTB时钟	
	
	//初始化B10
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource9,GPIO_AF_TIM11); 	//GPIOB8复用为定时器10	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        		//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;			//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      		//推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        		//上拉
	GPIO_Init(GPIOB,&GPIO_InitStructure);               		//初始化PA2/3
	
	TIM_TimeBaseStructure.TIM_Prescaler=168-1;  									//定时器分频
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 	//向上计数模式
	TIM_TimeBaseStructure.TIM_Period=20000-1;   									//自动重装载值 2.0ms
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM11,&TIM_TimeBaseStructure);							//初始化定时器10
	
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 							//选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 	//比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 			//输出极性:TIM输出比较极性高
	
	//初始化TIM11 Channel1 PWM模式	 
	TIM_OC1Init(TIM11, &TIM_OCInitStructure);  					//根据T指定的参数初始化外设TIM11OC1
	TIM_OC1PreloadConfig(TIM11, TIM_OCPreload_Enable);  	//使能TIM11在CCR1上的预装载寄存器
 
   TIM_ARRPreloadConfig(TIM11,ENABLE);	//ARPE使能 
	TIM_Cmd(TIM11, ENABLE);  						//使能TIM11
	TIM11->CCR1 =  1200; 
}


void Gun_TIM_Init(void)
{
  //枪1编码器部分利用定时器3 A项:PA6 B项:PA7
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;  
  TIM_ICInitTypeDef TIM_ICInitStructure;  
  GPIO_InitTypeDef GPIO_InitStructure;
  TIM_OCInitTypeDef TIM_OCInitStructure;
  
   
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  GPIO_PinAFConfig(GPIOA,GPIO_PinSource6,GPIO_AF_TIM3);
  GPIO_PinAFConfig(GPIOA,GPIO_PinSource7,GPIO_AF_TIM3);
  
  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
  
  TIM_TimeBaseStructure.TIM_Prescaler = 0x0; // No prescaling 
  TIM_TimeBaseStructure.TIM_Period = 60000; 
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;   
  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
  
  TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI1, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
  TIM_ICStructInit(&TIM_ICInitStructure);
  TIM_ICInitStructure.TIM_ICFilter = 0;
  TIM_ICInit(TIM3, &TIM_ICInitStructure);
  
  // Clear all pending interrupts
  TIM_ClearFlag(TIM3, TIM_FLAG_Update);
  TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);

  
  TIM_Cmd(TIM3, ENABLE); 
  
   //枪2编码器部分利用定时器4 A项:PB6 B项:PB7
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
 
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  GPIO_PinAFConfig(GPIOB,GPIO_PinSource6,GPIO_AF_TIM4);
  GPIO_PinAFConfig(GPIOB,GPIO_PinSource7,GPIO_AF_TIM4);
  

  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
  
  TIM_TimeBaseStructure.TIM_Prescaler = 0x0; // No prescaling 
  TIM_TimeBaseStructure.TIM_Period = 60000; 
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;   
  TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
  
  TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI1, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
  TIM_ICStructInit(&TIM_ICInitStructure);
  TIM_ICInitStructure.TIM_ICFilter = 0;
  TIM_ICInit(TIM4, &TIM_ICInitStructure);
  
  // Clear all pending interrupts
  TIM_ClearFlag(TIM4, TIM_FLAG_Update);
  TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);

  TIM_Cmd(TIM4, ENABLE); 

  //----------------- 定时器2初始化 电调 周期2.0ms A2/3 -> 电调  ------------------------

	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);  		//TIM2时钟使能    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 	//使能PORTF时钟	
	
	//A2/3不可写在一句话中
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_TIM2); 	//GPIOA2复用为定时器2
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_TIM2); 	//GPIOA3复用为定时器2	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;	//GPIOA2/3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        		//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;			//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      		//推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        		//上拉
	GPIO_Init(GPIOA,&GPIO_InitStructure);               		//初始化PA2/3
	
	TIM_TimeBaseStructure.TIM_Prescaler=84-1;  									//定时器分频
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 	//向上计数模式
	TIM_TimeBaseStructure.TIM_Period=2000-1;   									//自动重装载值 2.0ms
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);							//初始化定时器3
	
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 							//选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 	//比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 			//输出极性:TIM输出比较极性高
	
	//初始化TIM2 Channel3 PWM模式	 
	TIM_OC3Init(TIM2, &TIM_OCInitStructure);  					//根据T指定的参数初始化外设TIM2OC3
	TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);  	//使能TIM2在CCR1上的预装载寄存器
 
	//初始化TIM2 Channel4 PWM模式	 
	TIM_OC4Init(TIM2, &TIM_OCInitStructure);  					//根据T指定的参数初始化外设TIM2OC4
	TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);  	//使能TIM2在CCR1上的预装载寄存器

   TIM_ARRPreloadConfig(TIM2,ENABLE);	//ARPE使能 
	TIM_Cmd(TIM2, ENABLE);  						//使能TIM2
	
	
	//-------- 定时器1初始化 舵机 周期20ms E9/11/13/14 -> 舵机阻挡1供弹1阻挡2供弹2  -------------
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);  		//TIM1时钟使能    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE); 	//使能PORTF时钟	
	
	//A2/3不可写在一句话中
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource9,GPIO_AF_TIM1); 	//GPIOE9复用为定时器1
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource11,GPIO_AF_TIM1); 	//GPIOE11复用为定时器1
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource13,GPIO_AF_TIM1); 	//GPIOE13复用为定时器1
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource14,GPIO_AF_TIM1); 	//GPIOE14复用为定时器1

	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_11 |\
								  GPIO_Pin_13 | GPIO_Pin_14;	//GPIOE9/11/12/14
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        		//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;			//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      		//推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;        		//上拉
	GPIO_Init(GPIOE,&GPIO_InitStructure);               		
	
	TIM_TimeBaseStructure.TIM_Prescaler=168-1;  									//定时器分频
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 	//向上计数模式
	TIM_TimeBaseStructure.TIM_Period=20000-1;   									//自动重装载值 20ms
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseStructure);							//初始化定时器3
	
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
   TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
   TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
   TIM_OCInitStructure.TIM_OCNIdleState =  TIM_OCIdleState_Reset;
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 							//选择定时器模式:TIM脉冲宽度调制模式1
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 	//比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 			//输出极性:TIM输出比较极性高

	//初始化TIM1 Channel1 PWM模式	 
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);  					//根据T指定的参数初始化外设TIM1OC3
	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);  	//使能TIM1在CCR1上的预装载寄存器
 
	//初始化TIM1 Channel2 PWM模式	 
	TIM_OC2Init(TIM1, &TIM_OCInitStructure);  					//根据T指定的参数初始化外设TIM1OC4
	TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);  	//使能TIM1在CCR1上的预装载寄存器

	//初始化TIM1 Channel3 PWM模式	 
	TIM_OC3Init(TIM1, &TIM_OCInitStructure);  					//根据T指定的参数初始化外设TIM1OC3
	TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);  	//使能TIM1在CCR1上的预装载寄存器
 
	//初始化TIM1 Channel4 PWM模式	 
	TIM_OC4Init(TIM1, &TIM_OCInitStructure);  					//根据T指定的参数初始化外设TIM1OC4
	TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);  	//使能TIM1在CCR1上的预装载寄存器

   TIM_ARRPreloadConfig(TIM1,ENABLE);	      //ARPE使能 
	TIM_Cmd(TIM1, ENABLE);  						//使能TIM1
	TIM_CtrlPWMOutputs(TIM1, ENABLE); 
   
}
