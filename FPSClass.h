#pragma once

#pragma comment(lib, "winmm.lib")

class FPSClass
{
public:
	FPSClass();
	FPSClass(const FPSClass& InOther);
	~FPSClass();

	void Initialize();
	void Frame();
	int GetFPS();

private:
	int FPS, Count;
	unsigned long StartTime;
};

