/******************************************************************************
 * @file     remote.c
 * @brief    RMң�������ն˳���
 * @author   Liyi
 * @version  V1.0
 * @date     2017.6.12
 * @note
 * Copyright (C) 2017 SLDX Limited. All rights reserved.
 *
 * @par			����ʹ�ò�����100000	û����żУ��	ֹͣλ1
 *					���ڽ��ܵ������� u8 RXBuff[18];	һ�����ݰ�18���ֽ�
 *	ժҪ
 ******************************************************************************/

#include "remote.h"

/**
  * @brief  ң�������ݴ���
  * @param  None
  * @retval rc RC�ṹ�����ͱ��������ͷ�ļ�
  * @note   RXBuff �������ݽ��ܻ���
  */
extern unsigned char RXBuff[];		//���ڽ��ջ���
RC remote_Data_process()
{	
	RC rc;		//����
	rc.R_x = 1024 - ((RXBuff[0]| (RXBuff[1] << 8)) & 0x07ff); //!< Channel 0
	rc.R_y = 1024 - (((RXBuff[1] >> 3) | (RXBuff[2] << 5)) & 0x07ff); //!< Channel 1
	rc.L_x = 1024 - (((RXBuff[2] >> 6) | (RXBuff[3] << 2) | (RXBuff[4] << 10))& 0x07ff);//!< Channel 2
	rc.L_y = 1024 - (((RXBuff[4] >> 1) | (RXBuff[5] << 7)) & 0x07ff); //!< Channel 3
	rc.sl = ((RXBuff[5] >> 4)& 0x000C) >> 2; //!< Switch left
	rc.sr = ((RXBuff[5] >> 4)& 0x0003); //!< Switch left
	
	rc.mouse_x = RXBuff[6] | (RXBuff[7] << 8); //!< Mouse X axis
	rc.mouse_y = RXBuff[8] | (RXBuff[9] << 8); //!< Mouse Y axis
	rc.mouse_z = RXBuff[10] | (RXBuff[11] << 8); //!< Mouse Z axis
	rc.mouse_l = RXBuff[12]; //!< Mouse Left Is Press ?
	rc.mouse_r = RXBuff[13]; //!< Mouse Right Is Press ?
	rc.key = RXBuff[14] | (RXBuff[15] << 8); //!< KeyBoard value
	return rc;
}
