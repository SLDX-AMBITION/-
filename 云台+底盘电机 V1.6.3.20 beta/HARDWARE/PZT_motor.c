#include "PZT_motor.h"
#include "can.h"
#include "remote.h"
#include "mpu6050.h"
#include "inv_mpu.h"

pid_type Yaw_PID_inloop;//偏航 速度pid
pid_type Pitch_PID_inloop;//俯仰 速度pid
pid_type Yaw_PID_outloop;//偏航 角度pid
pid_type Pitch_PID_outloop;//俯仰 角度pid

State Yaw_state,Pitch_state;
State_ Yaw_state_;
int a;
int16_t Yaw_PID_out,Pitch_PID_out;
float Yaw_remote,Pitch_remote;
u8 Init;
//采样时间：
#define arr14 50-1;
#define psc14 840-1;

//YAW
//角度环：
#define Angle_P_Y 9.2
#define Angle_I_Y 0                                    
#define Angle_D_Y 0
//速度环：
#define Speed_P_Y 22
#define Speed_I_Y 0
#define Speed_D_Y 6

//Pitch
//角度环：
#define Angle_P_P 23
#define Angle_I_P 0
#define Angle_D_P 3
//速度环：
#define Speed_P_P 5
#define Speed_I_P 0
#define Speed_D_P 0
float roll,pitch;

extern RC rc;
u8 Can_buf_S[8];//发送canbuf bit: 0,1->Yaw;2,3->Pitch;

/**
  * @brief  云台初始化函数
  * @param  
  * @retval 
  * @note   None
  */
void PZT_Init()
{
//<---------------定时器14 PID计算 数据采样 --------------------->
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
 	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14,ENABLE);  ///使能TIM14时钟
	
    TIM_TimeBaseInitStructure.TIM_Period = arr14; 	//自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler = psc14;  //定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM14,&TIM_TimeBaseInitStructure);//初始化TIM14
	
	TIM_ITConfig(TIM14,TIM_IT_Update,ENABLE); //允许定时器14更新中断
	TIM_Cmd(TIM14,ENABLE); //使能定时�1�
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM8_TRG_COM_TIM14_IRQn; //定时器14中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x00; //抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x01; //子优先级1
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
//<---------------PID 初始化函数--------------------------------->
	PID_Init(&Yaw_PID_inloop,Speed_P_Y,Speed_I_Y,Speed_D_Y);  //偏航速度环
	PID_Init(&Yaw_PID_outloop,Angle_P_Y,Angle_I_Y,Angle_D_Y);  //偏航角度环
	PID_Init(&Pitch_PID_inloop,Speed_P_P,Speed_I_P,Speed_D_P);//俯仰速度环
	PID_Init(&Pitch_PID_outloop,Angle_P_P,Angle_I_P,Angle_D_P);			//俯仰角度环
//<---------------云台电机双轴归零------------------------------>
	Yaw_state.position.coder_prestore = 5500;
	Pitch_state.position.coder_prestore = 5040;
}


///**
//  * @brief  云台控制函数
//  * @param  Yaw->俯仰控制 Pitch->翻滚控制
//  * @retval 
//  * @note   None
//  */

//void PZT_Yaw_Pitch_Ctrl(int Y, int P)
//{
//	Yaw_PID_outloop.Set = Y;
//	Pitch_PID_outloop.Set = P;
//}

/**
  * @brief  云台PID周期处理函数
  * @param  
  * @retval 
  * @note   一定频率进行pid计算和对云台的控制
  */
u8 flag = 0;
void TIM8_TRG_COM_TIM14_IRQHandler(void)
{
	
	if(TIM_GetITStatus(TIM14,TIM_IT_Update)==SET) //溢出中断
	{
		if(Init)
		{
			Yaw_state.position.coder_raw = Can_buf_R[4][0]<<8 | Can_buf_R[4][1] ;	//机械角度范围0-8191(0x1FFF)
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
			
	//		Yaw_PID_inloop.Set = rc.R_x ;//内环输入值
	//		Pitch_PID_inloop.Set = rc.R_y ;//内环输入值
			
			Yaw_PID_out = -PID_realize_incremental(&Yaw_PID_inloop, Gyro_z);
			Pitch_PID_out = PID_realize_incremental(&Pitch_PID_inloop, Gyro_y) + 600;
			
			Yaw_PID_out = (Yaw_PID_out<5000) ? Yaw_PID_out : 5000;//限幅输出
			Yaw_PID_out = (Yaw_PID_out>-5000) ? Yaw_PID_out : -5000;
			Pitch_PID_out = (Pitch_PID_out<5000) ? Pitch_PID_out : 5000;
			Pitch_PID_out = (Pitch_PID_out>-5000) ? Pitch_PID_out : -5000;
			
	/////////////应急////////////应急////////////////应急//////////////////////////		
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
			
			
			Yaw_state.position.coder_raw = Can_buf_R[4][0]<<8 | Can_buf_R[4][1] ;	//机械角度范围0-8191(0x1FFF)
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
			
			Yaw_PID_out = (Yaw_PID_out<5000) ? Yaw_PID_out : 5000;//限幅输出
			Yaw_PID_out = (Yaw_PID_out>-5000) ? Yaw_PID_out : -5000;
			Pitch_PID_out = (Pitch_PID_out<5000) ? Pitch_PID_out : 5000;
			Pitch_PID_out = (Pitch_PID_out>-5000) ? Pitch_PID_out : -5000;
			
	/////////////应急////////////应急////////////////应急//////////////////////////		
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
	TIM_ClearITPendingBit(TIM14,TIM_IT_Update);  //清除中断标志位
}
/**
  * @brief  初始化云台数据处理
  * @param  
  * @retval 
  * @note   
  */
void PZT_data_Process(State *a)
{	
	a->position.coder_diff = a->position.coder_raw - a->position.coder_last;
	if(a->position.coder_diff < -7500)  //防止编码器偏差过大
	{
		a->position.coder_cnt++;
	}else if(a->position.coder_diff > 7500)
	{
		a->position.coder_cnt--;
	}
	a->position.now = (float)(a->position.coder_raw - a->position.coder_prestore)*360/8192 \
                      + a->position.coder_cnt*360;      //计算实际角度
	
	a->position.coder_last = a->position.coder_raw;
	
//	Yaw_state.Current = Can_buf_R[4][2]<<8| Can_buf_R[4][3];  //实际转矩电流值
//	Pitch_state.Current = Can_buf_R[5][2]<<8 | Can_buf_R[5][3];

//	Yaw_state.Current = (Yaw_state.Current<30000)?Yaw_state.Current:(Yaw_state.Current-65535);
//	Pitch_state.Current = (Pitch_state.Current<30000)?Pitch_state.Current:(Pitch_state.Current-65535);          
}

/**
  * @brief  云台数据处理
  * @param  
  * @retval 
  * @note   
  */
void PZT_data_Process_(State_ *a)
{	
	a->position.coder_diff = a->position.coder_raw - a->position.coder_last;
	if(a->position.coder_diff < -300)  //防止编码器偏差过大
	{
		a->position.coder_cnt++;
	}else if(a->position.coder_diff > 300)
	{
		a->position.coder_cnt--;
	}
	a->position.now = a->position.coder_raw + a->position.coder_cnt*360;      //计算实际角度
	
	a->position.coder_last = a->position.coder_raw;
	
//	Yaw_state.Current = Can_buf_R[4][2]<<8| Can_buf_R[4][3];  //实际转矩电流值
//	Pitch_state.Current = Can_buf_R[5][2]<<8 | Can_buf_R[5][3];

//	Yaw_state.Current = (Yaw_state.Current<30000)?Yaw_state.Current:(Yaw_state.Current-65535);
//	Pitch_state.Current = (Pitch_state.Current<30000)?Pitch_state.Current:(Pitch_state.Current-65535);          
}


/**
  * @brief  pid初始化函数
  * @param  pid结构体 P I D三个参数
  * @retval 无
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
  * @brief  角度环pid函数 采用位置式算法
  * @param  pid结构体 速度实际值
  * @retval 返回pid输出 连接速度环
  * @note   采用位置式算法 
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


