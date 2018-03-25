#ifndef _PZT_
#define _PZT_
#include "sys.h"

extern u8 Init;

typedef struct{
    float Set;    //定义设定值
    int Out; //输出值
	float Real; //编码器采样值
    float err;         //定义偏差值
    int err_next; //上一次偏差值
    int err_last; //最上次偏差值
	int integral; //误差累计
    float Kp,Ki,Kd;    //定义 
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
	}position;//位置角度
	
	float speed;//角速度
	float Current;//电流
	
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
	}position;//位置角度
	
	float speed;//角速度
	float Current;//电流
	
}State_;
extern State Yaw_state;
void PZT_Init(void);
void PID_Init(pid_type *PID_x, float p, float i, float d);
int  PID_realize_incremental(pid_type *PID_x, float r);
void PZT_data_Process(State *a);
void PZT_data_Process_(State_ *a);
#endif
