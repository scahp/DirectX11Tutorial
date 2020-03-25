#include "stdafx.h"
#include "FPSClass.h"
#include <mmsystem.h>

FPSClass::FPSClass()
{

}

FPSClass::FPSClass(const FPSClass& InOther)
{

}

FPSClass::~FPSClass()
{

}

void FPSClass::Initialize()
{
	FPS = 0;
	Count = 0;
	StartTime = timeGetTime();
}

void FPSClass::Frame()
{
	++Count;

	if (timeGetTime() >= (StartTime + 1000))
	{
		FPS = Count;
		Count = 0;
		StartTime = timeGetTime();
	}
}

int FPSClass::GetFPS()
{
	return FPS;
}
