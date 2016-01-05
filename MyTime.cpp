#include "MyDirectX.h"
#include "MyClasses.h"


bool Calc_Seconds(int seconds)
{
	double starttime = 0;
	double Seconds_Count = 0;
	if (GetTickCount() - starttime> 1000)
	{
		starttime = GetTickCount();
		Seconds_Count += 0.00096;
	}
	if (Seconds_Count > seconds)
	{
		return true;
	}
	return false;
}

void Time::Calc_Game_Time()
{
	double starttime = 0.0;
	if (GetTickCount() - starttime > 1000)
	{
		starttime = GetTickCount();
		Game_Time += 0.00096;
	}
}

float Time::Get_Seconds()
{
	return (int)Seconds_Count;
}

double Time::Get_Game_Time()
{
	return Game_Time;
}