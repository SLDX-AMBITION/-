#include "can.h"
#include "delay.h"
#include "usart.h"
//////////////////////////////////////////
//RM2018 云台实验程序
//STM32F4工程模板-库函数版本
//版本 1.0.10.16
//初步实验
//作者：沈理电协Ambition Wx
//CAN数据收发
//外部变量 
//u8 Can_buf[7][8];	0-3电机->1234; 4->Pitch 5->Yaw ;拨弹轮电机->6
///////////////////////////////////////////

//CAN初始化
//tsjw:重新同步跳跃时间单元.范围:CAN_SJW_1tq~ CAN_SJW_4tq
//tbs2:时间段2的时间单元.   范围:CAN_BS2_1tq~CAN_BS2_8tq;
//tbs1:时间段1的时间单元.   范围:CAN_BS1_1tq ~CAN_BS1_16tq
//brp :波特率分频器.范围:1~1024; tq=(brp)*tpclk1
//波特率=Fpclk1/((tbs1+1+tbs2+1+1)*brp);
//mode:CAN_Mode_Normal,普通模式;CAN_Mode_LoopBack,回环模式;
//Fpclk1的时钟在初始化的时候设置为42M,如果设置CAN1_Mode_Init(CAN_SJW_1tq,CAN_BS2_6tq,CAN_BS1_7tq,6,CAN_Mode_LoopBack);
//则波特率为:42M/((6+7+1)*6)=500Kbps
//返回值:0,初始化OK;
//    其他,初始化失败; 
 u8  Can_buf_R[6][8];//	0-3电机->1234; 4->Pitch 5->Yaw ;拨弹轮电机->6

u8 CAN1_Mode_Init(u8 tsjw,u8 tbs2,u8 tbs1,u16 brp,u8 mode)
{

  	GPIO_InitTypeDef GPIO_InitStructure; 
	CAN_InitTypeDef        CAN_InitStructure;
  	CAN_FilterInitTypeDef  CAN_FilterInitStructure;
#if CAN1_RX0_INT_ENABLE 
    NVIC_InitTypeDef  NVIC_InitStructure;
#endif
    //使能相关时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//使能PORTA时钟	                   											 

  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);//使能CAN1时钟	
	
    //初始化GPIO
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11| GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
    GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化PA11,PA12
	
	  //引脚复用映射配置
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource11,GPIO_AF_CAN1); //GPIOA11复用为CAN1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource12,GPIO_AF_CAN1); //GPIOA12复用为CAN1
	  
  	//CAN单元设置

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
	
	CAN_ITConfig(CAN1,CAN_IT_FMP0,ENABLE);//FIFO0消息挂号中断允许.		    
  
  	NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX0_IRQn;
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;     // 主优先级为1
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;            // 次优先级为0
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_Init(&NVIC_InitStructure);
#endif
	return 0;
}   
 
#if CAN1_RX0_INT_ENABLE	//使能RX0中断
//中断服务函数			    
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

//can发送一组数据(固定格式:ID为0X12,标准帧,数据帧)	
//len:数据长度(最大为8)				     
//msg:数据指针,最大为8个字节.
//返回值:0,成功;
//		 其他,失败;
u8 CAN1_Send_Msg(u8* msg,u8 len)
{	
	u8 mbox;
	u16 i=0;
	CanTxMsg TxMessage;
	TxMessage.StdId=0x200;	 // 标准标识符为0
	TxMessage.ExtId=0x0;	 // 设置扩展标示符（29位）
	TxMessage.IDE=0;		  // 使用扩展标识符
	TxMessage.RTR=0;		  // 消息类型为数据帧，一帧8位
	TxMessage.DLC=len;							 // 发送两帧信息
	for(i=0;i<len;i++)
	TxMessage.Data[i]=msg[i];				 // 第一帧信息          
	mbox= CAN_Transmit(CAN1, &TxMessage);   
	i=0;
	while((CAN_TransmitStatus(CAN1, mbox)==CAN_TxStatus_Failed)&&(i<0XFFF))i++;	//等待发送结束
	if(i>=0XFFF)return 1;
	return 0;		
}
//can发送一组数据(固定格式:ID为0X12,标准帧,数据帧)	
//len:数据长度(最大为8)				     
//msg:数据指针,最大为8个字节.
//返回值:0,成功;
//		 其他,失败;
u8 CAN1_Send_PZT_Msg(u8* msg,u8 len)
{	
	u8 mbox;
	u16 i=0;
	CanTxMsg TxMessage;
	TxMessage.StdId=0x1FF;	 // 标准标识符为0
	TxMessage.ExtId=0x0;	   // 设置扩展标示符（29位）
	TxMessage.IDE=0;		  // 使用扩展标识符
	TxMessage.RTR=0;		  // 消息类型为数据帧，一帧8位
	TxMessage.DLC=len;							 // 发送两帧信息
	for(i=0;i<4;i++)
	    TxMessage.Data[i]=msg[i];				 // 第一帧信息   
	TxMessage.Data[6]=0x00;
	mbox = CAN_Transmit(CAN1, &TxMessage);   
	i=0;
	while((CAN_TransmitStatus(CAN1, mbox)==CAN_TxStatus_Failed)&&(i<0XFFF))
	    i++;	//等待发送结束
	if(i>=0XFFF)return 1;
	return 0;		
}
//can口接收数据查询
//buf:数据缓存区;	 
//返回值:0,无数据被收到;
//		 其他,接收的数据长度;
u8 CAN1_Receive_Msg(u8 buf[2][8])
{		   		   
 	u16 i,n;
	CanRxMsg RxMessage;
    if( CAN_MessagePending(CAN1,CAN_FIFO0)==0)return 0;		//没有接收到数据,直接退出 
	 for(i = 0; i < 3; i++)
	 {
		while (RxMessage.StdId != (0x201 + i))
		{
			CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);//读取数据
	//		printf("%d",i);
		}
		for(n=0;n<8;n++)
			buf[i][n] = RxMessage.Data[n];  
	 }
	return 0;	
}










