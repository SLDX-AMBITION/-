#include "chassis_motor.h"
#include "can.h"
#include "PZT_motor.h"
#include "remote.h"
#include "chassis_motor.h"
#include "PZT_motor.h"
pid_type_cm CM1_PID,CM2_PID,CM3_PID,CM4_PID,SPIN_PID;
int16_t CM_Real_Speed[4];
extern RC rc;
//采样时间：
#define arr13 200-1
#define psc13 840-1
#define ERROR_MOTOR -150

#define CM_PID_P 8
#define CM_PID_I 0
#define CM_PID_D 1

#define SPIN_PID_P 2
#define SPIN_PID_I 0
#define SPIN_PID_D 1

#define CM_LIMITE 2000

void CM_Init()
{
//<---------------定时器13 PID计算 数据采样 --------------------->
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
 	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM13,ENABLE);  ///使能TIM13时钟
	
    TIM_TimeBaseInitStructure.TIM_Period = arr13; 	//自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler = psc13;  //定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM13,&TIM_TimeBaseInitStructure);//初始化TIM13
	
	TIM_ITConfig(TIM13,TIM_IT_Update,ENABLE); //允许定时器13更新中断
	TIM_Cmd(TIM13,ENABLE); //使能定时�1�
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM8_UP_TIM13_IRQn ; //定时器13中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //抢占优先级2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x02; //子优先级1
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
//<---------------PID 初始化函数--------------------------------->
	PID_Init_cm(&CM1_PID,CM_PID_P,CM_PID_I,CM_PID_D);
	PID_Init_cm(&CM2_PID,CM_PID_P,CM_PID_I,CM_PID_D);
	PID_Init_cm(&CM3_PID,CM_PID_P,CM_PID_I,CM_PID_D);
	PID_Init_cm(&CM4_PID,CM_PID_P,CM_PID_I,CM_PID_D);
	PID_Init_cm(&SPIN_PID,SPIN_PID_P,SPIN_PID_I,SPIN_PID_D);
}

void mecanum_Resolving(int *a ,int *b,int *c,int *d, int z)
{
	*a = z - rc.R_x - rc.L_x;
	*b = -(z + rc.R_x + rc.L_x);
	*c = -(z - rc.R_x + rc.L_x);
	*d = z + rc.R_x - rc.L_x;
}

void TIM8_UP_TIM13_IRQHandler(void)
{
	u8 Can_buf_s_cm[8];
	u8 i;
	int cm1_out,cm2_out,cm3_out,cm4_out;
	if(TIM_GetITStatus(TIM13,TIM_IT_Update)==SET) //溢出中断
	{
		CM_Real_Speed[0] = Can_buf_R[0][2]<<8 | Can_buf_R[0][3];
		CM_Real_Speed[0] = CM_Real_Speed[0]<32767 ? CM_Real_Speed[0] : CM_Real_Speed[0]-65535;
		CM_Real_Speed[1] = Can_buf_R[1][2]<<8 | Can_buf_R[1][3];
		CM_Real_Speed[1] = CM_Real_Speed[1]<32767 ? CM_Real_Speed[1] : CM_Real_Speed[1]-65535;
		CM_Real_Speed[2] = Can_buf_R[2][2]<<8 | Can_buf_R[2][3];
		CM_Real_Speed[2] = CM_Real_Speed[2]<32767 ? CM_Real_Speed[2] : CM_Real_Speed[2]-65535;
		CM_Real_Speed[3] = Can_buf_R[3][2]<<8 | Can_buf_R[3][3];
		CM_Real_Speed[3] = CM_Real_Speed[3]<32767 ? CM_Real_Speed[3] : CM_Real_Speed[3]-65535;
		
		for(i = 0; i < 4; i++)
		{
			CM_Real_Speed[i] /= 13;
		}
		
        mecanum_Resolving(&CM1_PID.Set ,&CM2_PID.Set, &CM3_PID.Set, &CM4_PID.Set, PID_realize_positiontype(&SPIN_PID,Yaw_state.position.now));//麦克纳姆轮解算
		
		cm1_out = PID_realize_positiontype(&CM1_PID,CM_Real_Speed[0]) + ERROR_MOTOR;
		cm2_out = PID_realize_positiontype(&CM2_PID,CM_Real_Speed[1]) + ERROR_MOTOR;
		cm3_out = PID_realize_positiontype(&CM3_PID,CM_Real_Speed[2]) + ERROR_MOTOR;
		cm4_out = PID_realize_positiontype(&CM4_PID,CM_Real_Speed[3]) + ERROR_MOTOR;
		
//		cm1_out = 1400;
//		cm2_out = 1400;
//		cm3_out = 1400;
//		cm4_out = 1400;
		
		cm1_out = cm1_out < CM_LIMITE ? cm1_out : CM_LIMITE;
		cm1_out = cm1_out > -CM_LIMITE ? cm1_out : -CM_LIMITE;
		cm2_out = cm2_out < CM_LIMITE ? cm2_out : CM_LIMITE;
		cm2_out = cm2_out > -CM_LIMITE ? cm2_out : -CM_LIMITE;
		cm3_out = cm3_out < CM_LIMITE ? cm3_out : CM_LIMITE;
		cm3_out = cm3_out > -CM_LIMITE ? cm3_out : -CM_LIMITE;
		cm4_out = cm4_out < CM_LIMITE ? cm4_out : CM_LIMITE;
		cm4_out = cm4_out > -CM_LIMITE ? cm4_out : -CM_LIMITE;
		
		if(rc.sl == 2)
		{
			Can_buf_s_cm[0] = cm1_out/256;Can_buf_s_cm[1] = cm1_out%256;
			Can_buf_s_cm[2] = cm2_out/256;Can_buf_s_cm[3] = cm2_out%256;
			Can_buf_s_cm[4] = cm3_out/256;Can_buf_s_cm[5] = cm3_out%256;
			Can_buf_s_cm[6] = cm4_out/256;Can_buf_s_cm[7] = cm4_out%256;
			CAN1_Send_Msg(Can_buf_s_cm,8);
		}
		else
		{
			Can_buf_s_cm[0] = 0;Can_buf_s_cm[1] = 0;
			Can_buf_s_cm[2] = 0;Can_buf_s_cm[3] = 0;
			Can_buf_s_cm[4] = 0;Can_buf_s_cm[5] = 0;
			Can_buf_s_cm[6] = 0;Can_buf_s_cm[7] = 0;
			CAN1_Send_Msg(Can_buf_s_cm,8);
		}
	}
	TIM_ClearITPendingBit(TIM13,TIM_IT_Update);  //清除中断标志位
}

/**
  * @brief  速度环pid函数 采用增量式算法
  * @param  pid结构体 速度实际值
  * @retval 返回pid输出 连接电流环
  * @note   采用增量式算法
  */
int PID_realize_positiontype(pid_type_cm *PID_x, float r)
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
/**
  * @brief  pid初始化函数
  * @param  pid结构体 P I D三个参数
  * @retval 无
  * @note   None
  */
void PID_Init_cm(pid_type_cm *PID_x, float p, float i, float d)
{
    PID_x->Set = 0;
    PID_x->Out = 0;
	PID_x->Real = 0;
    PID_x->err = 0;
    PID_x->err_last = 0;
	PID_x->err_next = 0;
	PID_x->integral = 0;
    PID_x->Kp = p;
    PID_x->Ki = i;
    PID_x->Kd = d;
}
