#ifndef _C_MOTOR_
#define _C_MOTOR_
#include "sys.h"

typedef struct{
    int Set;    //定义设定值
    int Out; //输出值
	int Real; //编码器采样值
    int err;         //定义偏差值
    int err_next; //上一次偏差值
    int err_last; //最上次偏差值
	int integral; //误差累计
    float Kp,Ki,Kd;    //定义 
}pid_type_cm;

void CM_Init(void);
int  PID_realize_positiontype(pid_type_cm *PID_x, float r);
void PID_Init_cm(pid_type_cm *PID_x, float p, float i, float d);
void mecanum_Resolving(int *a ,int *b,int *c,int *d, int z);

#endif
