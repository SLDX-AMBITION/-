#include "timer.h"

void TIM5_Int_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);  ///ʹ��TIM4ʱ��
	
  TIM_TimeBaseInitStructure.TIM_Period = arr; 	//�Զ���װ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //��ʱ����Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM5,&TIM_TimeBaseInitStructure);//��ʼ��TIM5
	
	TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE); //����ʱ��4�����ж�
	TIM_Cmd(TIM5,ENABLE); //ʹ�ܶ�ʱ��4
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM5_IRQn; //��ʱ��4�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; //��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; //�����ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);	
}
/*************************************
* 	ʹ��TIM14��TIM7
* 	TIM14��������pid
*	TIM7����ǹpid
*
*************************************/
void Move_PID_TIM_Init(u16 arr,u16 psc)
{
	//TIM14 ����PID
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
 	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14,ENABLE);  ///ʹ��TIM14ʱ��
	
  TIM_TimeBaseInitStructure.TIM_Period = arr; 	//�Զ���װ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //��ʱ����Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM14,&TIM_TimeBaseInitStructure);//��ʼ��TIM14
	
	TIM_ITConfig(TIM14,TIM_IT_Update,ENABLE); //����ʱ��14�����ж�
	TIM_Cmd(TIM14,ENABLE); //ʹ�ܶ�ʱ�1�
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM8_TRG_COM_TIM14_IRQn; //��ʱ��14�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x01; //�����ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
}
void Gun_PID_TIM_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
		
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7,ENABLE);  ///ʹ��TIM7ʱ��
	
  TIM_TimeBaseInitStructure.TIM_Period = arr; 	//�Զ���װ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler = psc;  //��ʱ����Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM7,&TIM_TimeBaseInitStructure);//��ʼ��TIM7
	
	TIM_ITConfig(TIM7,TIM_IT_Update,ENABLE); //����ʱ��14�����ж�
	TIM_Cmd(TIM7,ENABLE); //ʹ�ܶ�ʱ�1�
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM7_IRQn ; //��ʱ��14�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x02; //�����ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
}
void ReceiveBall_TIM_Init(int PWM)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;  
	TIM_ICInitTypeDef TIM_ICInitStructure;  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM10,ENABLE);  		//TIM10ʱ��ʹ��    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); 	//ʹ��PORTBʱ��	
	
	//��ʼ��B10
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource8,GPIO_AF_TIM10); 	//GPIOB8����Ϊ��ʱ��10	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        		//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;			//�ٶ�100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      		//���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        		//����
	GPIO_Init(GPIOB,&GPIO_InitStructure);               		//��ʼ��PA2/3
	
	TIM_TimeBaseStructure.TIM_Prescaler=84-1;  									//��ʱ����Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 	//���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_Period=1000-1;   									//�Զ���װ��ֵ 2.0ms
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM10,&TIM_TimeBaseStructure);							//��ʼ����ʱ��10
	
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 							//ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 	//�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 			//�������:TIM����Ƚϼ��Ը�
	
	//��ʼ��TIM10 Channel3 PWMģʽ	 
	TIM_OC1Init(TIM10, &TIM_OCInitStructure);  					//����Tָ���Ĳ�����ʼ������TIM10OC1
	TIM_OC1PreloadConfig(TIM10, TIM_OCPreload_Enable);  	//ʹ��TIM10��CCR1�ϵ�Ԥװ�ؼĴ���
 
   TIM_ARRPreloadConfig(TIM10,ENABLE);	//ARPEʹ�� 
	TIM_Cmd(TIM10, ENABLE);  						//ʹ��TIM10
	TIM10->CCR1 = PWM; //����
	
}
void SelectBall_TIM_Init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;  
	TIM_ICInitTypeDef TIM_ICInitStructure;  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM11,ENABLE);  		//TIM11ʱ��ʹ��    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); 	//ʹ��PORTBʱ��	
	
	//��ʼ��B10
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource9,GPIO_AF_TIM11); 	//GPIOB8����Ϊ��ʱ��10	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        		//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;			//�ٶ�100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      		//���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        		//����
	GPIO_Init(GPIOB,&GPIO_InitStructure);               		//��ʼ��PA2/3
	
	TIM_TimeBaseStructure.TIM_Prescaler=168-1;  									//��ʱ����Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 	//���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_Period=20000-1;   									//�Զ���װ��ֵ 2.0ms
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM11,&TIM_TimeBaseStructure);							//��ʼ����ʱ��10
	
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 							//ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 	//�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 			//�������:TIM����Ƚϼ��Ը�
	
	//��ʼ��TIM11 Channel1 PWMģʽ	 
	TIM_OC1Init(TIM11, &TIM_OCInitStructure);  					//����Tָ���Ĳ�����ʼ������TIM11OC1
	TIM_OC1PreloadConfig(TIM11, TIM_OCPreload_Enable);  	//ʹ��TIM11��CCR1�ϵ�Ԥװ�ؼĴ���
 
   TIM_ARRPreloadConfig(TIM11,ENABLE);	//ARPEʹ�� 
	TIM_Cmd(TIM11, ENABLE);  						//ʹ��TIM11
	TIM11->CCR1 =  1200; 
}


void Gun_TIM_Init(void)
{
  //ǹ1�������������ö�ʱ��3 A��:PA6 B��:PA7
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
  
   //ǹ2�������������ö�ʱ��4 A��:PB6 B��:PB7
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

  //----------------- ��ʱ��2��ʼ�� ��� ����2.0ms A2/3 -> ���  ------------------------

	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);  		//TIM2ʱ��ʹ��    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 	//ʹ��PORTFʱ��	
	
	//A2/3����д��һ�仰��
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_TIM2); 	//GPIOA2����Ϊ��ʱ��2
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_TIM2); 	//GPIOA3����Ϊ��ʱ��2	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;	//GPIOA2/3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        		//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;			//�ٶ�100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      		//���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        		//����
	GPIO_Init(GPIOA,&GPIO_InitStructure);               		//��ʼ��PA2/3
	
	TIM_TimeBaseStructure.TIM_Prescaler=84-1;  									//��ʱ����Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 	//���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_Period=2000-1;   									//�Զ���װ��ֵ 2.0ms
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);							//��ʼ����ʱ��3
	
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 							//ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 	//�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 			//�������:TIM����Ƚϼ��Ը�
	
	//��ʼ��TIM2 Channel3 PWMģʽ	 
	TIM_OC3Init(TIM2, &TIM_OCInitStructure);  					//����Tָ���Ĳ�����ʼ������TIM2OC3
	TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);  	//ʹ��TIM2��CCR1�ϵ�Ԥװ�ؼĴ���
 
	//��ʼ��TIM2 Channel4 PWMģʽ	 
	TIM_OC4Init(TIM2, &TIM_OCInitStructure);  					//����Tָ���Ĳ�����ʼ������TIM2OC4
	TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);  	//ʹ��TIM2��CCR1�ϵ�Ԥװ�ؼĴ���

   TIM_ARRPreloadConfig(TIM2,ENABLE);	//ARPEʹ�� 
	TIM_Cmd(TIM2, ENABLE);  						//ʹ��TIM2
	
	
	//-------- ��ʱ��1��ʼ�� ��� ����20ms E9/11/13/14 -> ����赲1����1�赲2����2  -------------
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);  		//TIM1ʱ��ʹ��    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE); 	//ʹ��PORTFʱ��	
	
	//A2/3����д��һ�仰��
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource9,GPIO_AF_TIM1); 	//GPIOE9����Ϊ��ʱ��1
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource11,GPIO_AF_TIM1); 	//GPIOE11����Ϊ��ʱ��1
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource13,GPIO_AF_TIM1); 	//GPIOE13����Ϊ��ʱ��1
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource14,GPIO_AF_TIM1); 	//GPIOE14����Ϊ��ʱ��1

	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_11 |\
								  GPIO_Pin_13 | GPIO_Pin_14;	//GPIOE9/11/12/14
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        		//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;			//�ٶ�100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      		//���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;        		//����
	GPIO_Init(GPIOE,&GPIO_InitStructure);               		
	
	TIM_TimeBaseStructure.TIM_Prescaler=168-1;  									//��ʱ����Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 	//���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_Period=20000-1;   									//�Զ���װ��ֵ 20ms
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseStructure);							//��ʼ����ʱ��3
	
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
   TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
   TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
   TIM_OCInitStructure.TIM_OCNIdleState =  TIM_OCIdleState_Reset;
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 							//ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ1
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 	//�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 			//�������:TIM����Ƚϼ��Ը�

	//��ʼ��TIM1 Channel1 PWMģʽ	 
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);  					//����Tָ���Ĳ�����ʼ������TIM1OC3
	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);  	//ʹ��TIM1��CCR1�ϵ�Ԥװ�ؼĴ���
 
	//��ʼ��TIM1 Channel2 PWMģʽ	 
	TIM_OC2Init(TIM1, &TIM_OCInitStructure);  					//����Tָ���Ĳ�����ʼ������TIM1OC4
	TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);  	//ʹ��TIM1��CCR1�ϵ�Ԥװ�ؼĴ���

	//��ʼ��TIM1 Channel3 PWMģʽ	 
	TIM_OC3Init(TIM1, &TIM_OCInitStructure);  					//����Tָ���Ĳ�����ʼ������TIM1OC3
	TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);  	//ʹ��TIM1��CCR1�ϵ�Ԥװ�ؼĴ���
 
	//��ʼ��TIM1 Channel4 PWMģʽ	 
	TIM_OC4Init(TIM1, &TIM_OCInitStructure);  					//����Tָ���Ĳ�����ʼ������TIM1OC4
	TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);  	//ʹ��TIM1��CCR1�ϵ�Ԥװ�ؼĴ���

   TIM_ARRPreloadConfig(TIM1,ENABLE);	      //ARPEʹ�� 
	TIM_Cmd(TIM1, ENABLE);  						//ʹ��TIM1
	TIM_CtrlPWMOutputs(TIM1, ENABLE); 
   
}
