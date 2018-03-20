#ifndef __CAN_H
#define __CAN_H	 
#include "sys.h"	    
//////////////////////////////////////////
//RM2018 云台实验程序
//STM32F4工程模板-库函数版本
//版本 1.0.10.16
//初步实验
//作者：沈理电协Ambition Wx
//CAN数据收发
//外部变量 
//u8 Can_buf[7][8];//	0-3电机->1234; 4->Pitch 5->Yaw ;拨弹轮电机->6
/////////////////////////////////////////// 
extern  u8 Can_buf_R[6][8];
	
//CAN1接收RX0中断使能
#define CAN1_RX0_INT_ENABLE	1		//0,不使能;1,使能.								    
										 							 				    
u8 CAN1_Mode_Init(u8 tsjw,u8 tbs2,u8 tbs1,u16 brp,u8 mode);//CAN初始化
 
u8 CAN1_Send_Msg(u8* msg,u8 len);						//发送数据

u8 CAN1_Receive_Msg(u8 buf[4][8]);							//接收数据

u8 CAN1_Send_PZT_Msg(u8* msg,u8 len);
#endif

















