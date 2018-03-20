/******************************************************************************
 * @file     remote.h
 * @brief    RM遥控器接收端程序
 * @author   Liyi
 * @version  V1.0
 * @date     2017.6.12
 * @note
 * Copyright (C) 2017 SLDX Limited. All rights reserved.
 *
 * @par				//对于键鼠数据详见PDF文档
 *	摘要
 ******************************************************************************/

#ifndef _REMOTE_H
#define _REMOTE_H

#include "usart.h"

typedef struct
{
	int16_t R_x;  //右手x		右1684 左364 中1024  1684-1024 = 1024 - 364 = 660
	int16_t R_y;	 //右手y    上1684 下364 中1024
	int16_t L_x;
	int16_t L_y;
	uint8_t sl;			//拨动左  上1 中3 下2
	uint8_t sr;
	
	uint16_t mouse_x;	//鼠标
	uint16_t mouse_y;
	uint16_t mouse_z;
	uint8_t mouse_l;	//鼠标按键
	uint8_t mouse_r;
	uint16_t key;			//键盘
}RC;

//extern RC rc;

RC remote_Data_process(void);	//返回RC结构体类型变量

#endif
