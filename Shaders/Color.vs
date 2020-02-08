﻿
cbuffer MatrixBuffer
{
	matrix WorldMatrix;
	matrix ViewMatrix;
	matrix ProjectionMatrix;
};

struct VertexInputType
{
	float4 Position : POSITION;
	float4 Color : COLOR;
};

struct PixelInputType
{
	float4 Position : SV_POSITION;
	float4 Color : COLOR;
};

PixelInputType ColorVertexShader(VertexInputType Input)
{
	PixelInputType Output;

	// 적절한 행렬 계산을 위해 위치 벡터를 4단위로 변경
	Input.Position.w = 1.0f;

	// 월드, 뷰 그리고 투영 행렬에 대한 정점의 위치를 계산합니다.
	Output.Position = mul(Input.Position, WorldMatrix);
	Output.Position = mul(Output.Position, ViewMatrix);
	Output.Position = mul(Output.Position, ProjectionMatrix);

	// 픽셀 쉐이더가 사용할 입력 색상
	Output.Color = Input.Color;

	return Output;
}
