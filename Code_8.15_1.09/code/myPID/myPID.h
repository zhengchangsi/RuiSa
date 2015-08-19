#ifndef _myPID_H_
#define _myPID_H_ 

typedef struct
{
	float P;
	float I;
	float D;
} PID;


extern PID PID_Coef;

float PID_Regulator(float NowValue);

extern float PID_Global;


#endif
