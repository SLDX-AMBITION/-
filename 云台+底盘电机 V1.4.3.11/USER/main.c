#include "stm32f4xx.h"
//#include "usart.h"
#include "delay.h"
#include "PZT_motor.h"
#include "can.h"
#include "delay.h"
#include "remote.h"
#include "mpu6050.h"
#include "chassis_motor.h"
//#include "dma.h"
//////////////////////////////////////////////////
//RM2018 ��̨ʵ�����
//�汾 1.2.1.5
//����ʵ��
//���ߣ������ЭAmbition Wx
//������
//////////////////////////////////////////////////

int main(void)
{
	Remote_uart1_init();
	delay_init(168);
	CAN1_Mode_Init(CAN_SJW_1tq,CAN_BS2_4tq,CAN_BS1_9tq,3,0);//CAN��ʼ����ͨģʽ,������1Mbps  

	
	MPU_Init();
	PZT_Init();
	CM_Init();
	while(1)
	{
	}
	
}
void TIM8_BRK_TIM12_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM12,TIM_IT_Update)==SET) //����ж�
	{
		//read_Gyrodate() ;
	}
	TIM_ClearITPendingBit(TIM12,TIM_IT_Update);  //����жϱ�־λ
}
