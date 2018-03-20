#include "mpu6050.h"
#include "sys.h"
#include "delay.h"
#include "usart.h"   
//////////////////////////////////////////////////
//RM2018 ÔÆÌ¨ÊµÑé³ÌĞò
//°æ±¾ 1.2.1.5
//³õ²½ÊµÑé
//×÷Õß£ºÉòÀíµçĞ­Ambition Wx
//³ÌĞò£ºmpu6050
//////////////////////////////////////////////////

//»¬¶¯ÂË²¨
//FIFO_manyÎª¿Éµ÷Á¿
#define many_wx 40

int zy[2][many_wx + 1];
short y_compensate,z_compensate;
float Gyro_z,Gyro_y;
void read_Gyrodate() 
{
    u8 i;
    int sum_y = 0, sum_z = 0;
	short y,z;
	MPU_Get_Gyroscope(&y,&z);
	zy[0][many_wx] = y - y_compensate;
	zy[1][many_wx] = z - z_compensate;
    for(i = 0; i < many_wx; i++) 
	{
      zy[0][i] = zy[0][i+1];
		zy[1][i] = zy[1][i+1];
		sum_y += zy[0][i];
		sum_z += zy[1][i];
    }
//	Gyro_z = (float)(sum_z/many_wx)/32.8f;
//	Gyro_y = (float)(sum_y/many_wx)/32.8f;
	Gyro_z = (float)(z - z_compensate)/130.0f;
	Gyro_y = (float)(y - y_compensate)/320.0f;
}


//³õÊ¼»¯MPU6050
//·µ»ØÖµ:0,³É¹¦
//    ÆäËû,´íÎó´úÂë


u8 MPU_Init(void)
{ 
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	u8 res;
	u8 i;
	short sum_y,sum_z;

	IIC_Init();//³õÊ¼»¯IIC×ÜÏß
	delay_ms(500);
	MPU_Write_Byte(MPU_PWR_MGMT1_REG,0X80);	//¸´Î»MPU6050
    delay_ms(200);
	MPU_Write_Byte(MPU_PWR_MGMT1_REG,0X00);	//»½ĞÑMPU6050 
	MPU_Set_Gyro_Fsr(0);					//ÍÓÂİÒÇ´«¸ĞÆ÷,¡À250dps
	MPU_Set_Accel_Fsr(0);					//¼ÓËÙ¶È´«¸ĞÆ÷,¡À2g
	MPU_Set_Rate(1000);						//ÉèÖÃ²ÉÑùÂÊ50Hz
	MPU_Write_Byte(MPU_INT_EN_REG,0X00);	//¹Ø±ÕËùÓĞÖĞ¶Ï
	MPU_Write_Byte(MPU_USER_CTRL_REG,0X00);	//I2CÖ÷Ä£Ê½¹Ø±Õ
	MPU_Write_Byte(MPU_FIFO_EN_REG,0X00);	//¹Ø±ÕFIFO
	MPU_Write_Byte(MPU_INTBP_CFG_REG,0X80);	//INTÒı½ÅµÍµçÆ½ÓĞĞ§
	res=MPU_Read_Byte(MPU_DEVICE_ID_REG);
	if(res==MPU_ADDR)//Æ÷¼şIDÕıÈ·
	{
		MPU_Write_Byte(MPU_PWR_MGMT1_REG,0X01);	//ÉèÖÃCLKSEL,PLL XÖáÎª²Î¿¼
		MPU_Write_Byte(MPU_PWR_MGMT2_REG,0X00);	//¼ÓËÙ¶ÈÓëÍÓÂİÒÇ¶¼¹¤×÷
		MPU_Set_Rate(1000);						//ÉèÖÃ²ÉÑùÂÊÎª50Hz
 	}else return 1;
	for(i=0;i<5;i++)
	{
		MPU_Get_Gyroscope(&sum_y,&sum_z);
		sum_y += sum_y;
		sum_z += sum_z;
		delay_ms(10);
	}
	y_compensate = sum_y/5 - 19;
	z_compensate = sum_z/5 - 19;
//	//<---------------¶¨Ê±Æ÷14 PID¼ÆËã Êı¾İ²ÉÑù --------------------->
//	
// 	
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM12,ENABLE);  ///Ê¹ÄÜTIM14Ê±ÖÓ
//	
//    TIM_TimeBaseInitStructure.TIM_Period = 100-1; 	//×Ô¶¯ÖØ×°ÔØÖµ
//	TIM_TimeBaseInitStructure.TIM_Prescaler = 840-1;  //¶¨Ê±Æ÷·ÖÆµ
//	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //ÏòÉÏ¼ÆÊıÄ£Ê½
//	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
//	
//	TIM_TimeBaseInit(TIM12,&TIM_TimeBaseInitStructure);//³õÊ¼»¯TIM14
//	
//	TIM_ITConfig(TIM12,TIM_IT_Update,ENABLE); //ÔÊĞí¶¨Ê±Æ÷14¸üĞÂÖĞ¶Ï
//	TIM_Cmd(TIM12,ENABLE); //Ê¹ÄÜ¶¨Ê±Æ1÷
//	
//	NVIC_InitStructure.NVIC_IRQChannel=TIM8_BRK_TIM12_IRQn; //¶¨Ê±Æ÷14ÖĞ¶Ï
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //ÇÀÕ¼ÓÅÏÈ¼¶1
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x00; //×ÓÓÅÏÈ¼¶1
//	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
//	NVIC_Init(&NVIC_InitStructure);
//	return 0;
}
//ÉèÖÃMPU6050ÍÓÂİÒÇ´«¸ĞÆ÷ÂúÁ¿³Ì·¶Î§
//fsr:0,¡À250dps;1,¡À500dps;2,¡À1000dps;3,¡À2000dps
//·µ»ØÖµ:0,ÉèÖÃ³É¹¦
//    ÆäËû,ÉèÖÃÊ§°Ü 
u8 MPU_Set_Gyro_Fsr(u8 fsr)
{
	return MPU_Write_Byte(MPU_GYRO_CFG_REG,fsr<<3);//ÉèÖÃÍÓÂİÒÇÂúÁ¿³Ì·¶Î§  
}
//ÉèÖÃMPU6050¼ÓËÙ¶È´«¸ĞÆ÷ÂúÁ¿³Ì·¶Î§
//fsr:0,¡À2g;1,¡À4g;2,¡À8g;3,¡À16g
//·µ»ØÖµ:0,ÉèÖÃ³É¹¦
//    ÆäËû,ÉèÖÃÊ§°Ü 
u8 MPU_Set_Accel_Fsr(u8 fsr)
{
	return MPU_Write_Byte(MPU_ACCEL_CFG_REG,fsr<<3);//ÉèÖÃ¼ÓËÙ¶È´«¸ĞÆ÷ÂúÁ¿³Ì·¶Î§  
}
//ÉèÖÃMPU6050µÄÊı×ÖµÍÍ¨ÂË²¨Æ÷
//lpf:Êı×ÖµÍÍ¨ÂË²¨ÆµÂÊ(Hz)
//·µ»ØÖµ:0,ÉèÖÃ³É¹¦
//    ÆäËû,ÉèÖÃÊ§°Ü 
u8 MPU_Set_LPF(u16 lpf)
{
	u8 data=0;
	if(lpf>=188)data=1;
	else if(lpf>=98)data=2;
	else if(lpf>=42)data=3;
	else if(lpf>=20)data=4;
	else if(lpf>=10)data=5;
	else data=6; 
	return MPU_Write_Byte(MPU_CFG_REG,data);//ÉèÖÃÊı×ÖµÍÍ¨ÂË²¨Æ÷  
}
//ÉèÖÃMPU6050µÄ²ÉÑùÂÊ(¼Ù¶¨Fs=1KHz)
//rate:4~1000(Hz)
//·µ»ØÖµ:0,ÉèÖÃ³É¹¦
//    ÆäËû,ÉèÖÃÊ§°Ü 
u8 MPU_Set_Rate(u16 rate)
{
	u8 data;
	if(rate>1000)rate=1000;
	if(rate<4)rate=4;
	data=1000/rate-1;
	data=MPU_Write_Byte(MPU_SAMPLE_RATE_REG,data);	//ÉèÖÃÊı×ÖµÍÍ¨ÂË²¨Æ÷
 	return MPU_Set_LPF(rate/2);	//×Ô¶¯ÉèÖÃLPFÎª²ÉÑùÂÊµÄÒ»°ë
}

//µÃµ½ÎÂ¶ÈÖµ
//·µ»ØÖµ:ÎÂ¶ÈÖµ(À©´óÁË100±¶)
short MPU_Get_Temperature(void)
{
    u8 buf[2]; 
    short raw;
	float temp;
	MPU_Read_Len(MPU_ADDR,MPU_TEMP_OUTH_REG,2,buf); 
    raw=((u16)buf[0]<<8)|buf[1];  
    temp=36.53+((double)raw)/340;  
    return temp*100;;
}
//µÃµ½ÍÓÂİÒÇÖµ(Ô­Ê¼Öµ)
//gx,gy,gz:ÍÓÂİÒÇx,y,zÖáµÄÔ­Ê¼¶ÁÊı(´ø·ûºÅ)
//·µ»ØÖµ:0,³É¹¦
//    ÆäËû,´íÎó´úÂë
u8 MPU_Get_Gyroscope(short *gy,short *gz)
{
	u8 buf[6],res; 
	res=MPU_Read_Len(MPU_ADDR,MPU_GYRO_XOUTH_REG,6,buf);
	if(res==0)
	{ 
		*gy=((u16)buf[0]<<8)|buf[1];  
		*gz=((u16)buf[4]<<8)|buf[5];

	} 
    return res;;
}

//µÃµ½¼ÓËÙ¶ÈÖµ(Ô­Ê¼Öµ)
//gx,gy,gz:ÍÓÂİÒÇx,y,zÖáµÄÔ­Ê¼¶ÁÊı(´ø·ûºÅ)
//·µ»ØÖµ:0,³É¹¦
//    ÆäËû,´íÎó´úÂë
u8 MPU_Get_Accelerometer(short *ax,short *ay,short *az)
{
    u8 buf[6],res;  
	res=MPU_Read_Len(MPU_ADDR,MPU_ACCEL_XOUTH_REG,6,buf);
	if(res==0)
	{
		*ax=((u16)buf[0]<<8)|buf[1];  
		*ay=((u16)buf[2]<<8)|buf[3];  
		*az=((u16)buf[4]<<8)|buf[5];
	} 	
    return res;;
}
//IICÁ¬ĞøĞ´
//addr:Æ÷¼şµØÖ· 
//reg:¼Ä´æÆ÷µØÖ·
//len:Ğ´Èë³¤¶È
//buf:Êı¾İÇø
//·µ»ØÖµ:0,Õı³£
//    ÆäËû,´íÎó´úÂë
u8 MPU_Write_Len(u8 addr,u8 reg,u8 len,u8 *buf)
{
	u8 i; 
    IIC_Start(); 
	IIC_Send_Byte((addr<<1)|0);//·¢ËÍÆ÷¼şµØÖ·+Ğ´ÃüÁî	
	if(IIC_Wait_Ack())	//µÈ´ıÓ¦´ğ
	{
		IIC_Stop();		 
		return 1;		
	}
    IIC_Send_Byte(reg);	//Ğ´¼Ä´æÆ÷µØÖ·
    IIC_Wait_Ack();		//µÈ´ıÓ¦´ğ
	for(i=0;i<len;i++)
	{
		IIC_Send_Byte(buf[i]);	//·¢ËÍÊı¾İ
		if(IIC_Wait_Ack())		//µÈ´ıACK
		{
			IIC_Stop();	 
			return 1;		 
		}		
	}    
    IIC_Stop();	 
	return 0;	
} 
//IICÁ¬Ğø¶Á
//addr:Æ÷¼şµØÖ·
//reg:Òª¶ÁÈ¡µÄ¼Ä´æÆ÷µØÖ·
//len:Òª¶ÁÈ¡µÄ³¤¶È
//buf:¶ÁÈ¡µ½µÄÊı¾İ´æ´¢Çø
//·µ»ØÖµ:0,Õı³£
//    ÆäËû,´íÎó´úÂë
u8 MPU_Read_Len(u8 addr,u8 reg,u8 len,u8 *buf)
{ 
 	IIC_Start(); 
	IIC_Send_Byte((addr<<1)|0);//·¢ËÍÆ÷¼şµØÖ·+Ğ´ÃüÁî	
	if(IIC_Wait_Ack())	//µÈ´ıÓ¦´ğ
	{
		IIC_Stop();		 
		return 1;		
	}
    IIC_Send_Byte(reg);	//Ğ´¼Ä´æÆ÷µØÖ·
    IIC_Wait_Ack();		//µÈ´ıÓ¦´ğ
    IIC_Start();
	IIC_Send_Byte((addr<<1)|1);//·¢ËÍÆ÷¼şµØÖ·+¶ÁÃüÁî	
    IIC_Wait_Ack();		//µÈ´ıÓ¦´ğ 
	while(len)
	{
		if(len==1)*buf=IIC_Read_Byte(0);//¶ÁÊı¾İ,·¢ËÍnACK 
		else *buf=IIC_Read_Byte(1);		//¶ÁÊı¾İ,·¢ËÍACK  
		len--;
		buf++; 
	}    
    IIC_Stop();	//²úÉúÒ»¸öÍ£Ö¹Ìõ¼ş 
	return 0;	
}
//IICĞ´Ò»¸ö×Ö½Ú 
//reg:¼Ä´æÆ÷µØÖ·
//data:Êı¾İ
//·µ»ØÖµ:0,Õı³£
//    ÆäËû,´íÎó´úÂë
u8 MPU_Write_Byte(u8 reg,u8 data) 				 
{ 
    IIC_Start(); 
	IIC_Send_Byte((MPU_ADDR<<1)|0);//·¢ËÍÆ÷¼şµØÖ·+Ğ´ÃüÁî	
	if(IIC_Wait_Ack())	//µÈ´ıÓ¦´ğ
	{
		IIC_Stop();		 
		return 1;		
	}
    IIC_Send_Byte(reg);	//Ğ´¼Ä´æÆ÷µØÖ·
    IIC_Wait_Ack();		//µÈ´ıÓ¦´ğ 
	IIC_Send_Byte(data);//·¢ËÍÊı¾İ
	if(IIC_Wait_Ack())	//µÈ´ıACK
	{
		IIC_Stop();	 
		return 1;		 
	}		 
    IIC_Stop();	 
	return 0;
}
//IIC¶ÁÒ»¸ö×Ö½Ú 
//reg:¼Ä´æÆ÷µØÖ· 
//·µ»ØÖµ:¶Áµ½µÄÊı¾İ
u8 MPU_Read_Byte(u8 reg)
{
	u8 res;
    IIC_Start(); 
	IIC_Send_Byte((MPU_ADDR<<1)|0);//·¢ËÍÆ÷¼şµØÖ·+Ğ´ÃüÁî	
	IIC_Wait_Ack();		//µÈ´ıÓ¦´ğ 
    IIC_Send_Byte(reg);	//Ğ´¼Ä´æÆ÷µØÖ·
    IIC_Wait_Ack();		//µÈ´ıÓ¦´ğ
    IIC_Start();
	IIC_Send_Byte((MPU_ADDR<<1)|1);//·¢ËÍÆ÷¼şµØÖ·+¶ÁÃüÁî	
    IIC_Wait_Ack();		//µÈ´ıÓ¦´ğ 
	res=IIC_Read_Byte(0);//¶ÁÈ¡Êı¾İ,·¢ËÍnACK 
    IIC_Stop();			//²úÉúÒ»¸öÍ£Ö¹Ìõ¼ş 
	return res;		
}


