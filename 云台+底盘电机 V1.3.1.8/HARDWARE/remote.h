/******************************************************************************
 * @file     remote.h
 * @brief    RMң�������ն˳���
 * @author   Liyi
 * @version  V1.0
 * @date     2017.6.12
 * @note
 * Copyright (C) 2017 SLDX Limited. All rights reserved.
 *
 * @par				//���ڼ����������PDF�ĵ�
 *	ժҪ
 ******************************************************************************/

#ifndef _REMOTE_H
#define _REMOTE_H

#include "usart.h"

typedef struct
{
	int16_t R_x;  //����x		��1684 ��364 ��1024  1684-1024 = 1024 - 364 = 660
	int16_t R_y;	 //����y    ��1684 ��364 ��1024
	int16_t L_x;
	int16_t L_y;
	uint8_t sl;			//������  ��1 ��3 ��2
	uint8_t sr;
	
	uint16_t mouse_x;	//���
	uint16_t mouse_y;
	uint16_t mouse_z;
	uint8_t mouse_l;	//��갴��
	uint8_t mouse_r;
	uint16_t key;			//����
}RC;

//extern RC rc;

RC remote_Data_process(void);	//����RC�ṹ�����ͱ���

#endif
