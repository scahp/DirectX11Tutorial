#include "stdafx.h"
#include "CPUClass.h"

CPUClass::CPUClass()
{

}

CPUClass::~CPUClass()
{

}

void CPUClass::Initialize()
{
	// CPU 사용을 풀링하는 쿼리 객체를 만듭니다.
	PDH_STATUS Status = PdhOpenQuery(nullptr, 0, &QueryHandle);
	if (Status != ERROR_SUCCESS)
		CanReadCPU = false;

	Status = PdhAddCounter(QueryHandle, TEXT("\\Processor(_Total)\\% processor time"), 0, &CounterHandle);
	if (Status != ERROR_SUCCESS)
		CanReadCPU = false;

	LastSampleTime = GetTickCount();
	CPUUsage = 0;
}

void CPUClass::Shutdown()
{
	if (CanReadCPU)
		PdhCloseQuery(QueryHandle);
}

void CPUClass::Frame()
{
	PDH_FMT_COUNTERVALUE Value;

	if (CanReadCPU)
	{
		if ((LastSampleTime + 1000) < GetTickCount())
		{
			LastSampleTime = GetTickCount();
			PdhCollectQueryData(QueryHandle);
			PdhGetFormattedCounterValue(CounterHandle, PDH_FMT_LONG, nullptr, &Value);
			CPUUsage = Value.longValue;
		}
	}
}

int CPUClass::GetCPUPercentage()
{
	int Usage = 0;
	if (CanReadCPU)
		Usage = (int)CPUUsage;
	return Usage;
}
