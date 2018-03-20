#include "can.h"
#include "delay.h"
#include "usart.h"
//////////////////////////////////////////
//RM2018 ��̨ʵ�����
//STM32F4����ģ��-�⺯���汾
//�汾 1.0.10.16
//����ʵ��
//���ߣ������ЭAmbition Wx
//CAN�����շ�
//�ⲿ���� 
//u8 Can_buf[7][8];	0-3���->1234; 4->Pitch 5->Yaw ;�����ֵ��->6
///////////////////////////////////////////

//CAN��ʼ��
//tsjw:����ͬ����Ծʱ�䵥Ԫ.��Χ:CAN_SJW_1tq~ CAN_SJW_4tq
//tbs2:ʱ���2��ʱ�䵥Ԫ.   ��Χ:CAN_BS2_1tq~CAN_BS2_8tq;
//tbs1:ʱ���1��ʱ�䵥Ԫ.   ��Χ:CAN_BS1_1tq ~CAN_BS1_16tq
//brp :�����ʷ�Ƶ��.��Χ:1~1024; tq=(brp)*tpclk1
//������=Fpclk1/((tbs1+1+tbs2+1+1)*brp);
//mode:CAN_Mode_Normal,��ͨģʽ;CAN_Mode_LoopBack,�ػ�ģʽ;
//Fpclk1��ʱ���ڳ�ʼ����ʱ������Ϊ42M,�������CAN1_Mode_Init(CAN_SJW_1tq,CAN_BS2_6tq,CAN_BS1_7tq,6,CAN_Mode_LoopBack);
//������Ϊ:42M/((6+7+1)*6)=500Kbps
//����ֵ:0,��ʼ��OK;
//    ����,��ʼ��ʧ��; 
 u8  Can_buf_R[6][8];//	0-3���->1234; 4->Pitch 5->Yaw ;�����ֵ��->6

u8 CAN1_Mode_Init(u8 tsjw,u8 tbs2,u8 tbs1,u16 brp,u8 mode)
{

  	GPIO_InitTypeDef GPIO_InitStructure; 
	CAN_InitTypeDef        CAN_InitStructure;
  	CAN_FilterInitTypeDef  CAN_FilterInitStructure;
#if CAN1_RX0_INT_ENABLE 
    NVIC_InitTypeDef  NVIC_InitStructure;
#endif
    //ʹ�����ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//ʹ��PORTAʱ��	                   											 

  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);//ʹ��CAN1ʱ��	
	
    //��ʼ��GPIO
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11| GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
    GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��PA11,PA12
	
	  //���Ÿ���ӳ������
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource11,GPIO_AF_CAN1); //GPIOA11����ΪCAN1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource12,GPIO_AF_CAN1); //GPIOA12����ΪCAN1
	  
  	//CAN��Ԫ����

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN2, ENABLE);
    
    //CAN1
    CAN_InitStructure.CAN_ABOM      =   ENABLE;
    CAN_InitStructure.CAN_AWUM      =   DISABLE;
    CAN_InitStructure.CAN_BS1       =   CAN_BS1_9tq;
    CAN_InitStructure.CAN_BS2       =   CAN_BS2_4tq;
    CAN_InitStructure.CAN_Mode      =   CAN_Mode_Normal;
    CAN_InitStructure.CAN_NART      =   DISABLE;
    CAN_InitStructure.CAN_Prescaler =   3;
    CAN_InitStructure.CAN_RFLM      =   DISABLE;
    CAN_InitStructure.CAN_SJW       =   CAN_SJW_1tq;
    CAN_InitStructure.CAN_TTCM      =   DISABLE;
    CAN_InitStructure.CAN_TXFP      =   DISABLE;
    CAN_Init(CAN1, &CAN_InitStructure);
    
    CAN_FilterInitStructure.CAN_FilterActivation    =   ENABLE;
    CAN_FilterInitStructure.CAN_FilterFIFOAssignment=   CAN_Filter_FIFO0;
    CAN_FilterInitStructure.CAN_FilterIdHigh        =   0x0000;
    CAN_FilterInitStructure.CAN_FilterIdLow         =   0x0000;
    CAN_FilterInitStructure.CAN_FilterMaskIdHigh    =   0x0000;
    CAN_FilterInitStructure.CAN_FilterMaskIdLow     =   0x0000;
    CAN_FilterInitStructure.CAN_FilterMode          =   CAN_FilterMode_IdMask;
    CAN_FilterInitStructure.CAN_FilterNumber        =   0;
    CAN_FilterInitStructure.CAN_FilterScale         =   CAN_FilterScale_32bit;
    CAN_FilterInit(&CAN_FilterInitStructure);
		
#if CAN1_RX0_INT_ENABLE
	
	CAN_ITConfig(CAN1,CAN_IT_FMP0,ENABLE);//FIFO0��Ϣ�Һ��ж�����.		    
  
  	NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX0_IRQn;
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;     // �����ȼ�Ϊ1
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;            // �����ȼ�Ϊ0
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_Init(&NVIC_InitStructure);
#endif
	return 0;
}   
 
#if CAN1_RX0_INT_ENABLE	//ʹ��RX0�ж�
//�жϷ�����			    
void CAN1_RX0_IRQHandler(void)
{
  	CanRxMsg RxMessage;
	u16 i;
	if (CAN_GetITStatus(CAN1,CAN_IT_FMP0)!= RESET)
	{
		CAN_ClearITPendingBit(CAN1, CAN_IT_FF0);
		CAN_ClearFlag(CAN1, CAN_FLAG_FF0);
		CAN_Receive(CAN1, 0, &RxMessage);
		switch(RxMessage.StdId)
		{
			case 0x201: 
			{
				for(i = 0; i < 4; i++)
					Can_buf_R[0][i] = RxMessage.Data[i];
			}break;
			case 0x202: 
			{
				for(i = 0; i < 4; i++)
					Can_buf_R[1][i] = RxMessage.Data[i];
			}break;
			case 0x203: 
			{
				for(i = 0; i < 4; i++)
					Can_buf_R[2][i] = RxMessage.Data[i];
			}break;
			case 0x204: 
			{
				for(i = 0; i < 4; i++)
					Can_buf_R[3][i] = RxMessage.Data[i];
			}break;
			case 0x205: 
			{
				for(i = 0; i < 8; i++)
					Can_buf_R[4][i] = RxMessage.Data[i];
			}break;
			case 0x206: 
			{
				for(i = 0; i < 8; i++)
					Can_buf_R[5][i] = RxMessage.Data[i];
			}break;	
		}
	}
}		
	
//		printf("%d  %d\n",Can_buf_R[5][0],Can_buf_R[5][1]);
#endif

//can����һ������(�̶���ʽ:IDΪ0X12,��׼֡,����֡)	
//len:���ݳ���(���Ϊ8)				     
//msg:����ָ��,���Ϊ8���ֽ�.
//����ֵ:0,�ɹ�;
//		 ����,ʧ��;
u8 CAN1_Send_Msg(u8* msg,u8 len)
{	
	u8 mbox;
	u16 i=0;
	CanTxMsg TxMessage;
	TxMessage.StdId=0x200;	 // ��׼��ʶ��Ϊ0
	TxMessage.ExtId=0x0;	 // ������չ��ʾ����29λ��
	TxMessage.IDE=0;		  // ʹ����չ��ʶ��
	TxMessage.RTR=0;		  // ��Ϣ����Ϊ����֡��һ֡8λ
	TxMessage.DLC=len;							 // ������֡��Ϣ
	for(i=0;i<len;i++)
	TxMessage.Data[i]=msg[i];				 // ��һ֡��Ϣ          
	mbox= CAN_Transmit(CAN1, &TxMessage);   
	i=0;
	while((CAN_TransmitStatus(CAN1, mbox)==CAN_TxStatus_Failed)&&(i<0XFFF))i++;	//�ȴ����ͽ���
	if(i>=0XFFF)return 1;
	return 0;		
}
//can����һ������(�̶���ʽ:IDΪ0X12,��׼֡,����֡)	
//len:���ݳ���(���Ϊ8)				     
//msg:����ָ��,���Ϊ8���ֽ�.
//����ֵ:0,�ɹ�;
//		 ����,ʧ��;
u8 CAN1_Send_PZT_Msg(u8* msg,u8 len)
{	
	u8 mbox;
	u16 i=0;
	CanTxMsg TxMessage;
	TxMessage.StdId=0x1FF;	 // ��׼��ʶ��Ϊ0
	TxMessage.ExtId=0x0;	   // ������չ��ʾ����29λ��
	TxMessage.IDE=0;		  // ʹ����չ��ʶ��
	TxMessage.RTR=0;		  // ��Ϣ����Ϊ����֡��һ֡8λ
	TxMessage.DLC=len;							 // ������֡��Ϣ
	for(i=0;i<4;i++)
	    TxMessage.Data[i]=msg[i];				 // ��һ֡��Ϣ   
	TxMessage.Data[6]=0x00;
	mbox = CAN_Transmit(CAN1, &TxMessage);   
	i=0;
	while((CAN_TransmitStatus(CAN1, mbox)==CAN_TxStatus_Failed)&&(i<0XFFF))
	    i++;	//�ȴ����ͽ���
	if(i>=0XFFF)return 1;
	return 0;		
}
//can�ڽ������ݲ�ѯ
//buf:���ݻ�����;	 
//����ֵ:0,�����ݱ��յ�;
//		 ����,���յ����ݳ���;
u8 CAN1_Receive_Msg(u8 buf[2][8])
{		   		   
 	u16 i,n;
	CanRxMsg RxMessage;
    if( CAN_MessagePending(CAN1,CAN_FIFO0)==0)return 0;		//û�н��յ�����,ֱ���˳� 
	 for(i = 0; i < 3; i++)
	 {
		while (RxMessage.StdId != (0x201 + i))
		{
			CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);//��ȡ����
	//		printf("%d",i);
		}
		for(n=0;n<8;n++)
			buf[i][n] = RxMessage.Data[n];  
	 }
	return 0;	
}










