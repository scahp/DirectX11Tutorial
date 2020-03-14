#pragma once

class PositionClass
{
public:
	void SetFrameTime(float InTime);
	void GetRotation(float& OutY);

	void TurnLeft(bool InKeyDown);
	void TurnRight(bool InKeyDown);
private:
	float FrameTime = 0.0f;
	float RotationY = 0.0f;
	float LeftTurnSpeed = 0.0f;
	float RightTurnSpeed = 0.0f;
};

