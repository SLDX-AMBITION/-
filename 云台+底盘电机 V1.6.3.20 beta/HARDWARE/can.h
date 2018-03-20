#ifndef __CAN_H
#define __CAN_H	 
#include "sys.h"	    
//////////////////////////////////////////
//RM2018 ��̨ʵ�����
//STM32F4����ģ��-�⺯���汾
//�汾 1.0.10.16
//����ʵ��
//���ߣ������ЭAmbition Wx
//CAN�����շ�
//�ⲿ���� 
//u8 Can_buf[7][8];//	0-3���->1234; 4->Pitch 5->Yaw ;�����ֵ��->6
/////////////////////////////////////////// 
extern  u8 Can_buf_R[6][8];
	
//CAN1����RX0�ж�ʹ��
#define CAN1_RX0_INT_ENABLE	1		//0,��ʹ��;1,ʹ��.								    
										 							 				    
u8 CAN1_Mode_Init(u8 tsjw,u8 tbs2,u8 tbs1,u16 brp,u8 mode);//CAN��ʼ��
 
u8 CAN1_Send_Msg(u8* msg,u8 len);						//��������

u8 CAN1_Receive_Msg(u8 buf[4][8]);							//��������

u8 CAN1_Send_PZT_Msg(u8* msg,u8 len);
#endif

















