#ifndef _TIMER_H
#define _TIMER_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//��ʱ�� ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/6/16
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	


void TIM4_Int_Init(u16 arr,u16 psc);
void Gun_TIM_Init(void);
void Gun_PID_TIM_Init(u16 arr,u16 psc);
void Move_PID_TIM_Init(u16 arr,u16 psc);
void TIM5_Int_Init(u16 arr,u16 psc);
void ReceiveBall_TIM_Init(int PWM);
void SelectBall_TIM_Init(void);                                                                   
#endif
