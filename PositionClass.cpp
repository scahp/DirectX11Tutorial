#include "stdafx.h"
#include "PositionClass.h"

void PositionClass::SetFrameTime(float InTime)
{
	FrameTime = InTime;
}

void PositionClass::GetRotation(float& OutY)
{
	OutY = RotationY;
}

void PositionClass::TurnLeft(bool InKeyDown)
{
	if (InKeyDown)
	{
		LeftTurnSpeed += FrameTime * 0.01f;
		if (LeftTurnSpeed > (FrameTime * 0.15f))
			LeftTurnSpeed = FrameTime * 0.15f;
	}
	else
	{
		LeftTurnSpeed -= FrameTime * 0.05f;
		if (LeftTurnSpeed < 0.0f)
			LeftTurnSpeed = 0.0f;
	}

	RotationY -= LeftTurnSpeed;
	if (RotationY < 0.0f)
		RotationY += 360.0f;
}

void PositionClass::TurnRight(bool InKeyDown)
{
	if (InKeyDown)
	{
		RightTurnSpeed += FrameTime * 0.01f;
		if (RightTurnSpeed > (FrameTime * 0.15f))
			RightTurnSpeed = FrameTime * 0.15f;
	}
	else
	{
		RightTurnSpeed -= FrameTime * 0.005f;
		if (RightTurnSpeed < 0.0f)
			RightTurnSpeed = 0.0f;
	}

	RotationY += RightTurnSpeed;
	if (RotationY > 360.0f)
		RotationY -= 360.0f;
}