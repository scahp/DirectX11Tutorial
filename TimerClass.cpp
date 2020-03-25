#include "stdafx.h"
#include "TimerClass.h"

bool TimerClass::Initialize()
{
	QueryPerformanceFrequency((LARGE_INTEGER*)&Frequency);
	if (Frequency == 0)
		return false;

	TicksPerMS = Frequency / 1000.0f;
	QueryPerformanceCounter((LARGE_INTEGER*)&StartTime);

	return true;
}

void TimerClass::Frame()
{
	INT64 CurrentTime = 0;
	QueryPerformanceCounter((LARGE_INTEGER*)&CurrentTime);

	float TimeDifference = (float)(CurrentTime - StartTime);
	FrameTime = TimeDifference / TicksPerMS;
	StartTime = CurrentTime;
}

float TimerClass::GetTime()
{
	return FrameTime;
}
