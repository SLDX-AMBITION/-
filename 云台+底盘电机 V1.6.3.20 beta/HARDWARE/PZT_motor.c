#include "PZT_motor.h"
#include "can.h"
#include "remote.h"
#include "mpu6050.h"
#include "inv_mpu.h"

pid_type Yaw_PID_inloop;//Æ«º½ ËÙ¶Èpid
pid_type Pitch_PID_inloop;//¸©Ñö ËÙ¶Èpid
pid_type Yaw_PID_outloop;//Æ«º½ ½Ç¶Èpid
pid_type Pitch_PID_outloop;//¸©Ñö ½Ç¶Èpid

State Yaw_state,Pitch_state;
State_ Yaw_state_;
int a;
int16_t Yaw_PID_out,Pitch_PID_out;
float Yaw_remote,Pitch_remote;
u8 Init;
//²ÉÑùÊ±¼ä£º
#define arr14 50-1;
#define psc14 840-1;

//YAW
//½Ç¶È»·£º
#define Angle_P_Y 9.2
#define Angle_I_Y 0                                    
#define Angle_D_Y 0
//ËÙ¶È»·£º
#define Speed_P_Y 22
#define Speed_I_Y 0
#define Speed_D_Y 6

//Pitch
//½Ç¶È»·£º
#define Angle_P_P 23
#define Angle_I_P 0
#define Angle_D_P 3
//ËÙ¶È»·£º
#define Speed_P_P 5
#define Speed_I_P 0
#define Speed_D_P 0
float roll,pitch;

extern RC rc;
u8 Can_buf_S[8];//·¢ËÍcanbuf bit: 0,1->Yaw;2,3->Pitch;

/**
  * @brief  ÔÆÌ¨³õÊ¼»¯º¯Êý
  * @param  
  * @retval 
  * @note   None
  */
void PZT_Init()
{
//<---------------¶¨Ê±Æ÷14 PID¼ÆËã Êý¾Ý²ÉÑù --------------------->
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
 	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14,ENABLE);  ///Ê¹ÄÜTIM14Ê±ÖÓ
	
    TIM_TimeBaseInitStructure.TIM_Period = arr14; 	//×Ô¶¯ÖØ×°ÔØÖµ
	TIM_TimeBaseInitStructure.TIM_Prescaler = psc14;  //¶¨Ê±Æ÷·ÖÆµ
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //ÏòÉÏ¼ÆÊýÄ£Ê½
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM14,&TIM_TimeBaseInitStructure);//³õÊ¼»¯TIM14
	
	TIM_ITConfig(TIM14,TIM_IT_Update,ENABLE); //ÔÊÐí¶¨Ê±Æ÷14¸üÐÂÖÐ¶Ï
	TIM_Cmd(TIM14,ENABLE); //Ê¹ÄÜ¶¨Ê±Æ1÷
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM8_TRG_COM_TIM14_IRQn; //¶¨Ê±Æ÷14ÖÐ¶Ï
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x00; //ÇÀÕ¼ÓÅÏÈ¼¶1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x01; //×ÓÓÅÏÈ¼¶1
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
//<---------------PID ³õÊ¼»¯º¯Êý--------------------------------->
	PID_Init(&Yaw_PID_inloop,Speed_P_Y,Speed_I_Y,Speed_D_Y);  //Æ«º½ËÙ¶È»·
	PID_Init(&Yaw_PID_outloop,Angle_P_Y,Angle_I_Y,Angle_D_Y);  //Æ«º½½Ç¶È»·
	PID_Init(&Pitch_PID_inloop,Speed_P_P,Speed_I_P,Speed_D_P);//¸©ÑöËÙ¶È»·
	PID_Init(&Pitch_PID_outloop,Angle_P_P,Angle_I_P,Angle_D_P);			//¸©Ñö½Ç¶È»·
//<---------------ÔÆÌ¨µç»úË«Öá¹éÁã------------------------------>
	Yaw_state.position.coder_prestore = 5500;
	Pitch_state.position.coder_prestore = 5040;
}


///**
//  * @brief  ÔÆÌ¨¿ØÖÆº¯Êý
//  * @param  Yaw->¸©Ñö¿ØÖÆ Pitch->·­¹ö¿ØÖÆ
//  * @retval 
//  * @note   None
//  */

//void PZT_Yaw_Pitch_Ctrl(int Y, int P)
//{
//	Yaw_PID_outloop.Set = Y;
//	Pitch_PID_outloop.Set = P;
//}

/**
  * @brief  ÔÆÌ¨PIDÖÜÆÚ´¦Àíº¯Êý
  * @param  
  * @retval 
  * @note   Ò»¶¨ÆµÂÊ½øÐÐpid¼ÆËãºÍ¶ÔÔÆÌ¨µÄ¿ØÖÆ
  */
u8 flag = 0;
void TIM8_TRG_COM_TIM14_IRQHandler(void)
{
	
	if(TIM_GetITStatus(TIM14,TIM_IT_Update)==SET) //Òç³öÖÐ¶Ï
	{
		if(Init)
		{
			Yaw_state.position.coder_raw = Can_buf_R[4][0]<<8 | Can_buf_R[4][1] ;	//»úÐµ½Ç¶È·¶Î§0-8191(0x1FFF)
			Pitch_state.position.coder_raw = Can_buf_R[5][0]<<8 | Can_buf_R[5][1] ;		
			read_Gyrodate() ;
			
			PZT_data_Process(&Yaw_state);
			PZT_data_Process(&Pitch_state);
			

			
			if (flag == 1)
			{
	//			Yaw_remote = Yaw_remote + (float)(rc.mouse_x /1000.0f);
				Yaw_remote = Yaw_remote + (float)(rc.L_x /4000.0f);
				Pitch_remote = Pitch_remote + (float)(rc.L_y /4000.0f);

				Yaw_PID_outloop.Set = Yaw_remote = Yaw_remote > 50 ? 50 : Yaw_remote ;
				Yaw_PID_outloop.Set = Yaw_remote = Yaw_remote < -50 ? -50 : Yaw_remote ;
				Pitch_PID_outloop.Set = Pitch_remote = Pitch_remote > 25 ? 25 : Pitch_remote ;
				Pitch_PID_outloop.Set = Pitch_remote = Pitch_remote < -35 ? -35 : Pitch_remote ;
				Yaw_PID_inloop.Set = PID_realize_incremental(&Yaw_PID_outloop, Yaw_state.position.now);
				Pitch_PID_inloop.Set = -PID_realize_incremental(&Pitch_PID_outloop, Pitch_state.position.now);
				flag = 0;
			}
			
			Yaw_PID_inloop.Set = (Yaw_PID_inloop.Set < 650) ? Yaw_PID_inloop.Set : 650;
			Yaw_PID_inloop.Set = (Yaw_PID_inloop.Set > -650) ? Yaw_PID_inloop.Set : -650;
			Pitch_PID_inloop.Set = (Pitch_PID_inloop.Set < 650) ? Pitch_PID_inloop.Set : 650;
			Pitch_PID_inloop.Set = (Pitch_PID_inloop.Set > -650) ? Pitch_PID_inloop.Set : -650;
			
	//		Yaw_PID_inloop.Set = rc.R_x ;//ÄÚ»·ÊäÈëÖµ
	//		Pitch_PID_inloop.Set = rc.R_y ;//ÄÚ»·ÊäÈëÖµ
			
			Yaw_PID_out = -PID_realize_incremental(&Yaw_PID_inloop, Gyro_z);
			Pitch_PID_out = PID_realize_incremental(&Pitch_PID_inloop, Gyro_y) + 600;
			
			Yaw_PID_out = (Yaw_PID_out<5000) ? Yaw_PID_out : 5000;//ÏÞ·ùÊä³ö
			Yaw_PID_out = (Yaw_PID_out>-5000) ? Yaw_PID_out : -5000;
			Pitch_PID_out = (Pitch_PID_out<5000) ? Pitch_PID_out : 5000;
			Pitch_PID_out = (Pitch_PID_out>-5000) ? Pitch_PID_out : -5000;
			
	/////////////Ó¦¼±////////////Ó¦¼±////////////////Ó¦¼±//////////////////////////		
			if(rc.sr == 2)
			{
				Can_buf_S[0] = Yaw_PID_out/256;Can_buf_S[1] = Yaw_PID_out%256;
				Can_buf_S[2] = Pitch_PID_out/256;Can_buf_S[3] = Pitch_PID_out%256;
				CAN1_Send_PZT_Msg(Can_buf_S,8);
			}
			else
			{
				Can_buf_S[0] = 0;Can_buf_S[1] = 0;
				Can_buf_S[2] = 0;Can_buf_S[3] = 0;
				CAN1_Send_PZT_Msg(Can_buf_S,8);
			}
			flag ++;
		}
		else
		{
			
			
			Yaw_state.position.coder_raw = Can_buf_R[4][0]<<8 | Can_buf_R[4][1] ;	//»úÐµ½Ç¶È·¶Î§0-8191(0x1FFF)
			Pitch_state.position.coder_raw = Can_buf_R[5][0]<<8 | Can_buf_R[5][1] ;		
			read_Gyrodate() ;
			
			PZT_data_Process_(&Yaw_state_);
			PZT_data_Process(&Yaw_state);
			PZT_data_Process(&Pitch_state);
			

			
			if (flag == 1)
			{
				while(mpu_dmp_get_data(&pitch, &roll, &Yaw_state_.position.coder_raw));
	//			Yaw_remote = Yaw_remote + (float)(rc.mouse_x /1000.0f);
				Yaw_remote = Yaw_remote + (float)(rc.L_x /4000.0f);
				Pitch_remote = Pitch_remote + (float)(rc.L_y /4000.0f);

				Yaw_PID_outloop.Set = Yaw_remote = Yaw_remote > 50 ? 50 : Yaw_remote ;
				Yaw_PID_outloop.Set = Yaw_remote = Yaw_remote < -50 ? -50 : Yaw_remote ;
				Pitch_PID_outloop.Set = Pitch_remote = Pitch_remote > 25 ? 25 : Pitch_remote ;
				Pitch_PID_outloop.Set = Pitch_remote = Pitch_remote < -35 ? -35 : Pitch_remote ;
				Yaw_PID_inloop.Set = PID_realize_incremental(&Yaw_PID_outloop, Yaw_state_.position.now);
				Pitch_PID_inloop.Set = -PID_realize_incremental(&Pitch_PID_outloop, Pitch_state.position.now);
				flag = 0;
			}
			
			Yaw_PID_inloop.Set = (Yaw_PID_inloop.Set < 650) ? Yaw_PID_inloop.Set : 650;
			Yaw_PID_inloop.Set = (Yaw_PID_inloop.Set > -650) ? Yaw_PID_inloop.Set : -650;
			Pitch_PID_inloop.Set = (Pitch_PID_inloop.Set < 650) ? Pitch_PID_inloop.Set : 650;
			Pitch_PID_inloop.Set = (Pitch_PID_inloop.Set > -650) ? Pitch_PID_inloop.Set : -650;
			
			Yaw_PID_out = -PID_realize_incremental(&Yaw_PID_inloop, Gyro_z);
			Pitch_PID_out = PID_realize_incremental(&Pitch_PID_inloop, Gyro_y) + 600;
			
			Yaw_PID_out = (Yaw_PID_out<5000) ? Yaw_PID_out : 5000;//ÏÞ·ùÊä³ö
			Yaw_PID_out = (Yaw_PID_out>-5000) ? Yaw_PID_out : -5000;
			Pitch_PID_out = (Pitch_PID_out<5000) ? Pitch_PID_out : 5000;
			Pitch_PID_out = (Pitch_PID_out>-5000) ? Pitch_PID_out : -5000;
			
	/////////////Ó¦¼±////////////Ó¦¼±////////////////Ó¦¼±//////////////////////////		
			if(rc.sr == 2)
			{
				Can_buf_S[0] = Yaw_PID_out/256;Can_buf_S[1] = Yaw_PID_out%256;
				Can_buf_S[2] = Pitch_PID_out/256;Can_buf_S[3] = Pitch_PID_out%256;
				CAN1_Send_PZT_Msg(Can_buf_S,8);
			}
			else
			{
				Can_buf_S[0] = 0;Can_buf_S[1] = 0;
				Can_buf_S[2] = 0;Can_buf_S[3] = 0;
				CAN1_Send_PZT_Msg(Can_buf_S,8);
			}
			flag ++;
		}
	}
	TIM_ClearITPendingBit(TIM14,TIM_IT_Update);  //Çå³ýÖÐ¶Ï±êÖ¾Î»
}
/**
  * @brief  ³õÊ¼»¯ÔÆÌ¨Êý¾Ý´¦Àí
  * @param  
  * @retval 
  * @note   
  */
void PZT_data_Process(State *a)
{	
	a->position.coder_diff = a->position.coder_raw - a->position.coder_last;
	if(a->position.coder_diff < -7500)  //·ÀÖ¹±àÂëÆ÷Æ«²î¹ý´ó
	{
		a->position.coder_cnt++;
	}else if(a->position.coder_diff > 7500)
	{
		a->position.coder_cnt--;
	}
	a->position.now = (float)(a->position.coder_raw - a->position.coder_prestore)*360/8192 \
                      + a->position.coder_cnt*360;      //¼ÆËãÊµ¼Ê½Ç¶È
	
	a->position.coder_last = a->position.coder_raw;
	
//	Yaw_state.Current = Can_buf_R[4][2]<<8| Can_buf_R[4][3];  //Êµ¼Ê×ª¾ØµçÁ÷Öµ
//	Pitch_state.Current = Can_buf_R[5][2]<<8 | Can_buf_R[5][3];

//	Yaw_state.Current = (Yaw_state.Current<30000)?Yaw_state.Current:(Yaw_state.Current-65535);
//	Pitch_state.Current = (Pitch_state.Current<30000)?Pitch_state.Current:(Pitch_state.Current-65535);          
}

/**
  * @brief  ÔÆÌ¨Êý¾Ý´¦Àí
  * @param  
  * @retval 
  * @note   
  */
void PZT_data_Process_(State_ *a)
{	
	a->position.coder_diff = a->position.coder_raw - a->position.coder_last;
	if(a->position.coder_diff < -300)  //·ÀÖ¹±àÂëÆ÷Æ«²î¹ý´ó
	{
		a->position.coder_cnt++;
	}else if(a->position.coder_diff > 300)
	{
		a->position.coder_cnt--;
	}
	a->position.now = a->position.coder_raw + a->position.coder_cnt*360;      //¼ÆËãÊµ¼Ê½Ç¶È
	
	a->position.coder_last = a->position.coder_raw;
	
//	Yaw_state.Current = Can_buf_R[4][2]<<8| Can_buf_R[4][3];  //Êµ¼Ê×ª¾ØµçÁ÷Öµ
//	Pitch_state.Current = Can_buf_R[5][2]<<8 | Can_buf_R[5][3];

//	Yaw_state.Current = (Yaw_state.Current<30000)?Yaw_state.Current:(Yaw_state.Current-65535);
//	Pitch_state.Current = (Pitch_state.Current<30000)?Pitch_state.Current:(Pitch_state.Current-65535);          
}


/**
  * @brief  pid³õÊ¼»¯º¯Êý
  * @param  pid½á¹¹Ìå P I DÈý¸ö²ÎÊý
  * @retval ÎÞ
  * @note   None
  */
void PID_Init(pid_type *PID_x, float p, float i, float d)
{
    PID_x->Set = 0;
    PID_x->Out = 0.0;
	PID_x->Real = 0;
    PID_x->err = 0;
    PID_x->err_last = 0;
	PID_x->err_next = 0;
	PID_x->integral = 0;
    PID_x->Kp = p;
    PID_x->Ki = i;
    PID_x->Kd = d;
}

/**
  * @brief  ½Ç¶È»·pidº¯Êý ²ÉÓÃÎ»ÖÃÊ½Ëã·¨
  * @param  pid½á¹¹Ìå ËÙ¶ÈÊµ¼ÊÖµ
  * @retval ·µ»ØpidÊä³ö Á¬½ÓËÙ¶È»·
  * @note   ²ÉÓÃÎ»ÖÃÊ½Ëã·¨ 
  */
int PID_realize_incremental(pid_type *PID_x, float r)
{
	
	PID_x->Real = r;	
    PID_x->err_next = PID_x->err;
	PID_x->err = PID_x->Set - PID_x->Real;
    PID_x->integral += PID_x->err;

	PID_x->Out = PID_x->Kp * PID_x->err +
							 PID_x->Ki * PID_x->integral +
							 PID_x->Kd * (PID_x->err - PID_x->err_next);

	return PID_x->Out;
}


