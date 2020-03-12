
cbuffer MatrixBuffer
{
	matrix WorldMatrix;
	matrix ViewMatrix;
	Matrix ProjectionMatrix;
};

cbuffer CameraBuffer
{
	float3 CameraPosition;
	float Padding;
};

struct VertexInputType
{
	float4 Position		: POSITION;
	float2 Tex			: TEXCOORD0;
	float3 Normal		: NORMAL;
};

struct PixelInputType
{
	float4 Position			: SV_POSITION;
	float2 Tex				: TEXCOORD0;
	float3 Normal			: NORMAL;
	float3 ViewDirection	: TEXCOORD1;
};

PixelInputType LightVertexShader(VertexInputType Input)
{
	PixelInputType Output;

	Input.Position.w = 1.0f;

	Output.Position = mul(Input.Position, WorldMatrix);
	Output.Position = mul(Output.Position, ViewMatrix);
	Output.Position = mul(Output.Position, ProjectionMatrix);

	Output.Tex = Input.Tex;
	Output.Normal = mul(Input.Normal, (float3x3)WorldMatrix);
	Output.Normal = normalize(Output.Normal);

	float4 WorldPosition = mul(Input.Position, WorldMatrix);
	WorldPosition /= WorldPosition.z;
	Output.ViewDirection = CameraPosition.xyz - WorldPosition.xyz;
	Output.ViewDirection = normalize(Output.ViewDirection);

	return Output;
}
