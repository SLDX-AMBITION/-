#include "timer.h"

void TIM5_Int_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);  ///Ê¹ÄÜTIM4Ê±ÖÓ
	
  TIM_TimeBaseInitStructure.TIM_Period = arr; 	//×Ô¶¯ÖØ×°ÔØÖµ
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //¶¨Ê±Æ÷·ÖÆµ
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //ÏòÉÏ¼ÆÊıÄ£Ê½
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM5,&TIM_TimeBaseInitStructure);//³õÊ¼»¯TIM5
	
	TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE); //ÔÊĞí¶¨Ê±Æ÷4¸üĞÂÖĞ¶Ï
	TIM_Cmd(TIM5,ENABLE); //Ê¹ÄÜ¶¨Ê±Æ÷4
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM5_IRQn; //¶¨Ê±Æ÷4ÖĞ¶Ï
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; //ÇÀÕ¼ÓÅÏÈ¼¶1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; //×ÓÓÅÏÈ¼¶1
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);	
}
/*************************************
* 	Ê¹ÓÃTIM14¼°TIM7
* 	TIM14ÓÃÓÚĞĞ×ßpid
*	TIM7ÓÃÓÚÇ¹pid
*
*************************************/
void Move_PID_TIM_Init(u16 arr,u16 psc)
{
	//TIM14 ÓÃÓÚPID
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
 	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14,ENABLE);  ///Ê¹ÄÜTIM14Ê±ÖÓ
	
  TIM_TimeBaseInitStructure.TIM_Period = arr; 	//×Ô¶¯ÖØ×°ÔØÖµ
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //¶¨Ê±Æ÷·ÖÆµ
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //ÏòÉÏ¼ÆÊıÄ£Ê½
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM14,&TIM_TimeBaseInitStructure);//³õÊ¼»¯TIM14
	
	TIM_ITConfig(TIM14,TIM_IT_Update,ENABLE); //ÔÊĞí¶¨Ê±Æ÷14¸üĞÂÖĞ¶Ï
	TIM_Cmd(TIM14,ENABLE); //Ê¹ÄÜ¶¨Ê±Æ1÷
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM8_TRG_COM_TIM14_IRQn; //¶¨Ê±Æ÷14ÖĞ¶Ï
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //ÇÀÕ¼ÓÅÏÈ¼¶1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x01; //×ÓÓÅÏÈ¼¶1
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
}
void Gun_PID_TIM_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
		
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7,ENABLE);  ///Ê¹ÄÜTIM7Ê±ÖÓ
	
  TIM_TimeBaseInitStructure.TIM_Period = arr; 	//×Ô¶¯ÖØ×°ÔØÖµ
	TIM_TimeBaseInitStructure.TIM_Prescaler = psc;  //¶¨Ê±Æ÷·ÖÆµ
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //ÏòÉÏ¼ÆÊıÄ£Ê½
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM7,&TIM_TimeBaseInitStructure);//³õÊ¼»¯TIM7
	
	TIM_ITConfig(TIM7,TIM_IT_Update,ENABLE); //ÔÊĞí¶¨Ê±Æ÷14¸üĞÂÖĞ¶Ï
	TIM_Cmd(TIM7,ENABLE); //Ê¹ÄÜ¶¨Ê±Æ1÷
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM7_IRQn ; //¶¨Ê±Æ÷14ÖĞ¶Ï
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //ÇÀÕ¼ÓÅÏÈ¼¶1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x02; //×ÓÓÅÏÈ¼¶1
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
}
void ReceiveBall_TIM_Init(int PWM)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;  
	TIM_ICInitTypeDef TIM_ICInitStructure;  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM10,ENABLE);  		//TIM10Ê±ÖÓÊ¹ÄÜ    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); 	//Ê¹ÄÜPORTBÊ±ÖÓ	
	
	//³õÊ¼»¯B10
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource8,GPIO_AF_TIM10); 	//GPIOB8¸´ÓÃÎª¶¨Ê±Æ÷10	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        		//¸´ÓÃ¹¦ÄÜ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;			//ËÙ¶È100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      		//ÍÆÍì¸´ÓÃÊä³ö
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        		//ÉÏÀ­
	GPIO_Init(GPIOB,&GPIO_InitStructure);               		//³õÊ¼»¯PA2/3
	
	TIM_TimeBaseStructure.TIM_Prescaler=84-1;  									//¶¨Ê±Æ÷·ÖÆµ
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 	//ÏòÉÏ¼ÆÊıÄ£Ê½
	TIM_TimeBaseStructure.TIM_Period=1000-1;   									//×Ô¶¯ÖØ×°ÔØÖµ 2.0ms
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM10,&TIM_TimeBaseStructure);							//³õÊ¼»¯¶¨Ê±Æ÷10
	
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 							//Ñ¡Ôñ¶¨Ê±Æ÷Ä£Ê½:TIMÂö³å¿í¶Èµ÷ÖÆÄ£Ê½2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 	//±È½ÏÊä³öÊ¹ÄÜ
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 			//Êä³ö¼«ĞÔ:TIMÊä³ö±È½Ï¼«ĞÔ¸ß
	
	//³õÊ¼»¯TIM10 Channel3 PWMÄ£Ê½	 
	TIM_OC1Init(TIM10, &TIM_OCInitStructure);  					//¸ù¾İTÖ¸¶¨µÄ²ÎÊı³õÊ¼»¯ÍâÉèTIM10OC1
	TIM_OC1PreloadConfig(TIM10, TIM_OCPreload_Enable);  	//Ê¹ÄÜTIM10ÔÚCCR1ÉÏµÄÔ¤×°ÔØ¼Ä´æÆ÷
 
   TIM_ARRPreloadConfig(TIM10,ENABLE);	//ARPEÊ¹ÄÜ 
	TIM_Cmd(TIM10, ENABLE);  						//Ê¹ÄÜTIM10
	TIM10->CCR1 = PWM; //ÊÕÇò
	
}
void SelectBall_TIM_Init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;  
	TIM_ICInitTypeDef TIM_ICInitStructure;  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM11,ENABLE);  		//TIM11Ê±ÖÓÊ¹ÄÜ    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); 	//Ê¹ÄÜPORTBÊ±ÖÓ	
	
	//³õÊ¼»¯B10
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource9,GPIO_AF_TIM11); 	//GPIOB8¸´ÓÃÎª¶¨Ê±Æ÷10	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        		//¸´ÓÃ¹¦ÄÜ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;			//ËÙ¶È100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      		//ÍÆÍì¸´ÓÃÊä³ö
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        		//ÉÏÀ­
	GPIO_Init(GPIOB,&GPIO_InitStructure);               		//³õÊ¼»¯PA2/3
	
	TIM_TimeBaseStructure.TIM_Prescaler=168-1;  									//¶¨Ê±Æ÷·ÖÆµ
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 	//ÏòÉÏ¼ÆÊıÄ£Ê½
	TIM_TimeBaseStructure.TIM_Period=20000-1;   									//×Ô¶¯ÖØ×°ÔØÖµ 2.0ms
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM11,&TIM_TimeBaseStructure);							//³õÊ¼»¯¶¨Ê±Æ÷10
	
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 							//Ñ¡Ôñ¶¨Ê±Æ÷Ä£Ê½:TIMÂö³å¿í¶Èµ÷ÖÆÄ£Ê½2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 	//±È½ÏÊä³öÊ¹ÄÜ
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 			//Êä³ö¼«ĞÔ:TIMÊä³ö±È½Ï¼«ĞÔ¸ß
	
	//³õÊ¼»¯TIM11 Channel1 PWMÄ£Ê½	 
	TIM_OC1Init(TIM11, &TIM_OCInitStructure);  					//¸ù¾İTÖ¸¶¨µÄ²ÎÊı³õÊ¼»¯ÍâÉèTIM11OC1
	TIM_OC1PreloadConfig(TIM11, TIM_OCPreload_Enable);  	//Ê¹ÄÜTIM11ÔÚCCR1ÉÏµÄÔ¤×°ÔØ¼Ä´æÆ÷
 
   TIM_ARRPreloadConfig(TIM11,ENABLE);	//ARPEÊ¹ÄÜ 
	TIM_Cmd(TIM11, ENABLE);  						//Ê¹ÄÜTIM11
	TIM11->CCR1 =  1200; 
}


void Gun_TIM_Init(void)
{
  //Ç¹1±àÂëÆ÷²¿·ÖÀûÓÃ¶¨Ê±Æ÷3 AÏî:PA6 BÏî:PA7
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
  
   //Ç¹2±àÂëÆ÷²¿·ÖÀûÓÃ¶¨Ê±Æ÷4 AÏî:PB6 BÏî:PB7
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

  //----------------- ¶¨Ê±Æ÷2³õÊ¼»¯ µçµ÷ ÖÜÆÚ2.0ms A2/3 -> µçµ÷  ------------------------

	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);  		//TIM2Ê±ÖÓÊ¹ÄÜ    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 	//Ê¹ÄÜPORTFÊ±ÖÓ	
	
	//A2/3²»¿ÉĞ´ÔÚÒ»¾ä»°ÖĞ
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_TIM2); 	//GPIOA2¸´ÓÃÎª¶¨Ê±Æ÷2
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_TIM2); 	//GPIOA3¸´ÓÃÎª¶¨Ê±Æ÷2	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;	//GPIOA2/3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        		//¸´ÓÃ¹¦ÄÜ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;			//ËÙ¶È100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      		//ÍÆÍì¸´ÓÃÊä³ö
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        		//ÉÏÀ­
	GPIO_Init(GPIOA,&GPIO_InitStructure);               		//³õÊ¼»¯PA2/3
	
	TIM_TimeBaseStructure.TIM_Prescaler=84-1;  									//¶¨Ê±Æ÷·ÖÆµ
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 	//ÏòÉÏ¼ÆÊıÄ£Ê½
	TIM_TimeBaseStructure.TIM_Period=2000-1;   									//×Ô¶¯ÖØ×°ÔØÖµ 2.0ms
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);							//³õÊ¼»¯¶¨Ê±Æ÷3
	
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 							//Ñ¡Ôñ¶¨Ê±Æ÷Ä£Ê½:TIMÂö³å¿í¶Èµ÷ÖÆÄ£Ê½2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 	//±È½ÏÊä³öÊ¹ÄÜ
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 			//Êä³ö¼«ĞÔ:TIMÊä³ö±È½Ï¼«ĞÔ¸ß
	
	//³õÊ¼»¯TIM2 Channel3 PWMÄ£Ê½	 
	TIM_OC3Init(TIM2, &TIM_OCInitStructure);  					//¸ù¾İTÖ¸¶¨µÄ²ÎÊı³õÊ¼»¯ÍâÉèTIM2OC3
	TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);  	//Ê¹ÄÜTIM2ÔÚCCR1ÉÏµÄÔ¤×°ÔØ¼Ä´æÆ÷
 
	//³õÊ¼»¯TIM2 Channel4 PWMÄ£Ê½	 
	TIM_OC4Init(TIM2, &TIM_OCInitStructure);  					//¸ù¾İTÖ¸¶¨µÄ²ÎÊı³õÊ¼»¯ÍâÉèTIM2OC4
	TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);  	//Ê¹ÄÜTIM2ÔÚCCR1ÉÏµÄÔ¤×°ÔØ¼Ä´æÆ÷

   TIM_ARRPreloadConfig(TIM2,ENABLE);	//ARPEÊ¹ÄÜ 
	TIM_Cmd(TIM2, ENABLE);  						//Ê¹ÄÜTIM2
	
	
	//-------- ¶¨Ê±Æ÷1³õÊ¼»¯ ¶æ»ú ÖÜÆÚ20ms E9/11/13/14 -> ¶æ»ú×èµ²1¹©µ¯1×èµ²2¹©µ¯2  -------------
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);  		//TIM1Ê±ÖÓÊ¹ÄÜ    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE); 	//Ê¹ÄÜPORTFÊ±ÖÓ	
	
	//A2/3²»¿ÉĞ´ÔÚÒ»¾ä»°ÖĞ
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource9,GPIO_AF_TIM1); 	//GPIOE9¸´ÓÃÎª¶¨Ê±Æ÷1
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource11,GPIO_AF_TIM1); 	//GPIOE11¸´ÓÃÎª¶¨Ê±Æ÷1
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource13,GPIO_AF_TIM1); 	//GPIOE13¸´ÓÃÎª¶¨Ê±Æ÷1
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource14,GPIO_AF_TIM1); 	//GPIOE14¸´ÓÃÎª¶¨Ê±Æ÷1

	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_11 |\
								  GPIO_Pin_13 | GPIO_Pin_14;	//GPIOE9/11/12/14
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        		//¸´ÓÃ¹¦ÄÜ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;			//ËÙ¶È100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      		//ÍÆÍì¸´ÓÃÊä³ö
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;        		//ÉÏÀ­
	GPIO_Init(GPIOE,&GPIO_InitStructure);               		
	
	TIM_TimeBaseStructure.TIM_Prescaler=168-1;  									//¶¨Ê±Æ÷·ÖÆµ
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 	//ÏòÉÏ¼ÆÊıÄ£Ê½
	TIM_TimeBaseStructure.TIM_Period=20000-1;   									//×Ô¶¯ÖØ×°ÔØÖµ 20ms
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseStructure);							//³õÊ¼»¯¶¨Ê±Æ÷3
	
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
   TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
   TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
   TIM_OCInitStructure.TIM_OCNIdleState =  TIM_OCIdleState_Reset;
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 							//Ñ¡Ôñ¶¨Ê±Æ÷Ä£Ê½:TIMÂö³å¿í¶Èµ÷ÖÆÄ£Ê½1
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 	//±È½ÏÊä³öÊ¹ÄÜ
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 			//Êä³ö¼«ĞÔ:TIMÊä³ö±È½Ï¼«ĞÔ¸ß

	//³õÊ¼»¯TIM1 Channel1 PWMÄ£Ê½	 
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);  					//¸ù¾İTÖ¸¶¨µÄ²ÎÊı³õÊ¼»¯ÍâÉèTIM1OC3
	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);  	//Ê¹ÄÜTIM1ÔÚCCR1ÉÏµÄÔ¤×°ÔØ¼Ä´æÆ÷
 
	//³õÊ¼»¯TIM1 Channel2 PWMÄ£Ê½	 
	TIM_OC2Init(TIM1, &TIM_OCInitStructure);  					//¸ù¾İTÖ¸¶¨µÄ²ÎÊı³õÊ¼»¯ÍâÉèTIM1OC4
	TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);  	//Ê¹ÄÜTIM1ÔÚCCR1ÉÏµÄÔ¤×°ÔØ¼Ä´æÆ÷

	//³õÊ¼»¯TIM1 Channel3 PWMÄ£Ê½	 
	TIM_OC3Init(TIM1, &TIM_OCInitStructure);  					//¸ù¾İTÖ¸¶¨µÄ²ÎÊı³õÊ¼»¯ÍâÉèTIM1OC3
	TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);  	//Ê¹ÄÜTIM1ÔÚCCR1ÉÏµÄÔ¤×°ÔØ¼Ä´æÆ÷
 
	//³õÊ¼»¯TIM1 Channel4 PWMÄ£Ê½	 
	TIM_OC4Init(TIM1, &TIM_OCInitStructure);  					//¸ù¾İTÖ¸¶¨µÄ²ÎÊı³õÊ¼»¯ÍâÉèTIM1OC4
	TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);  	//Ê¹ÄÜTIM1ÔÚCCR1ÉÏµÄÔ¤×°ÔØ¼Ä´æÆ÷

   TIM_ARRPreloadConfig(TIM1,ENABLE);	      //ARPEÊ¹ÄÜ 
	TIM_Cmd(TIM1, ENABLE);  						//Ê¹ÄÜTIM1
	TIM_CtrlPWMOutputs(TIM1, ENABLE); 
   
}
