#include "stm32f4xx.h"
//#include "usart.h"
#include "delay.h"
#include "PZT_motor.h"
#include "can.h"
#include "delay.h"
#include "remote.h"
#include "mpu6050.h"
#include "chassis_motor.h"
#include "inv_mpu.h"
//#include "dma.h"
//////////////////////////////////////////////////
//RM2018 ��̨ʵ�����
//�汾 1.2.1.5
//����ʵ��
//���ߣ������ЭAmbition Wx
//������
//////////////////////////////////////////////////
void ledInit();
u8 s;
int main(void)
{
	Init = 1;
	Remote_uart2_init();
	delay_init(168);
	MPU_Init();
	CAN1_Mode_Init(CAN_SJW_1tq,CAN_BS2_4tq,CAN_BS1_9tq,3,0);//CAN��ʼ����ͨģʽ,������1Mbps  
	PZT_Init();
	CM_Init();
	
	s = mpu_dmp_init();
	delay_ms(1000);
	Init = 0;
	ledInit();
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
void ledInit()
{
	GPIO_InitTypeDef GPIO_InitS; 
	RCC_AHB1PeriphClockCmd(RCC_AHB1ENR_GPIODEN, ENABLE);
	GPIO_InitS.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitS.GPIO_OType = GPIO_OType_PP;//�������
    GPIO_InitS.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
    GPIO_InitS.GPIO_PuPd = GPIO_PuPd_UP;//����
	GPIO_Init(GPIOD, &GPIO_InitS);
	GPIO_SetBits(GPIOD, GPIO_Pin_11);
}