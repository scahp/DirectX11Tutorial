#pragma once

class TimerClass
{
public:
	TimerClass() {}
	~TimerClass() {}

	bool Initialize();
	void Frame();

	float GetTime();

private:
	INT64 Frequency = 0;
	float TicksPerMS = 0;
	INT64 StartTime = 0;
	float FrameTime = 0;
};

