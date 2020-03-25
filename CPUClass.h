#pragma once

#pragma comment(lib, "pdh.lib")
#include <pdh.h>

class CPUClass
{
public:
	CPUClass();
	~CPUClass();

	void Initialize();
	void Shutdown();
	void Frame();
	int GetCPUPercentage();

private:
	bool CanReadCPU = true;
	HQUERY QueryHandle;
	HCOUNTER CounterHandle;
	unsigned long LastSampleTime = 0;
	long CPUUsage = 0;
};

