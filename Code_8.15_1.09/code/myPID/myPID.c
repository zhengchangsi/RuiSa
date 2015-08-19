#include "stm32f10x.h"
#include "myPID.h"

PID PID_Coef = {0.2, 0.005, 0};//{0.5, 0.01, 0};

float PID_Global = 0;

float PID_Regulator(float NowValue)
{
	float Result = 0;
	static float LastError = 0;
	float NowError = 0;
	static float SumError = 0;

	NowError = PID_Global - NowValue;
	
	SumError += NowError;
	
	Result = PID_Coef.P*NowError + PID_Coef.I*SumError + PID_Coef.D*(NowError-LastError);
	
	LastError = NowError;
	
	return Result;
}







