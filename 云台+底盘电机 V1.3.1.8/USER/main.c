#include "stm32f4xx.h"
#include "usart.h"
#include "delay.h"
#include "PZT_motor.h"
#include "can.h"
#include "delay.h"
#include "remote.h"
#include "mpu6050.h"
#include "chassis_motor.h"
//////////////////////////////////////////////////
//RM2018 云台实验程序
//版本 1.2.1.5
//初步实验
//作者：沈理电协Ambition Wx
//主函数
//////////////////////////////////////////////////
RC rc;
int main(void)
{
	uart_init(100000);
	delay_init(168);
	CAN1_Mode_Init(CAN_SJW_1tq,CAN_BS2_4tq,CAN_BS1_9tq,3,0);//CAN初始化普通模式,波特率1Mbps  
	MPU_Init();
	PZT_Init();
	CM_Init();
	while(1)
	{
		rc = remote_Data_process();
		read_Gyrodate() ;
	}
	
}
void TIM8_BRK_TIM12_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM12,TIM_IT_Update)==SET) //溢出中断
	{
		
	}
	TIM_ClearITPendingBit(TIM12,TIM_IT_Update);  //清除中断标志位
}