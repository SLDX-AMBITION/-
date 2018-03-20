#ifndef _PZT_
#define _PZT_
#include "sys.h"

typedef struct{
    float Set;    //�����趨ֵ
    int Out; //���ֵ
	float Real; //����������ֵ
    float err;         //����ƫ��ֵ
    int err_next; //��һ��ƫ��ֵ
    int err_last; //���ϴ�ƫ��ֵ
	int integral; //����ۼ�
    float Kp,Ki,Kd;    //���� 
}pid_type;

typedef struct{
	
	struct{
        u16 coder_raw;
		u16 coder_last;
		u16 coder_prestore;
		int16_t coder;
		int16_t coder_diff;
		int16_t coder_cnt;
		float now;
	}position;//λ�ýǶ�
	
	float speed;//���ٶ�
	float Current;//����
	
}State;

void PZT_Init(void);
void PID_Init(pid_type *PID_x, float p, float i, float d);
int  PID_realize_incremental(pid_type *PID_x, float r);
void PZT_data_Process(State *a);

#endif
