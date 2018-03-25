#ifndef _PZT_
#define _PZT_
#include "sys.h"

extern u8 Init;

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

typedef struct{
	
	struct{
        float coder_raw;
		float coder_last;
		float coder_prestore;
		float coder;
		float coder_diff;
		int16_t coder_cnt;
		float now;
	}position;//λ�ýǶ�
	
	float speed;//���ٶ�
	float Current;//����
	
}State_;
extern State Yaw_state;
void PZT_Init(void);
void PID_Init(pid_type *PID_x, float p, float i, float d);
int  PID_realize_incremental(pid_type *PID_x, float r);
void PZT_data_Process(State *a);
void PZT_data_Process_(State_ *a);
#endif
