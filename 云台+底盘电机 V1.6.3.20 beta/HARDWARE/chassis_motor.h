#ifndef _C_MOTOR_
#define _C_MOTOR_
#include "sys.h"

typedef struct{
    int Set;    //�����趨ֵ
    int Out; //���ֵ
	int Real; //����������ֵ
    int err;         //����ƫ��ֵ
    int err_next; //��һ��ƫ��ֵ
    int err_last; //���ϴ�ƫ��ֵ
	int integral; //����ۼ�
    float Kp,Ki,Kd;    //���� 
}pid_type_cm;

void CM_Init(void);
int  PID_realize_positiontype(pid_type_cm *PID_x, float r);
void PID_Init_cm(pid_type_cm *PID_x, float p, float i, float d);
void mecanum_Resolving(int *a ,int *b,int *c,int *d, int z);

#endif
