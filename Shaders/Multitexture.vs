
cbuffer MatrixBuffer
{
	matrix WorldMatrix;
	matrix ViewMatrix;
	matrix ProjectionMatrix;
};

struct VertexInputType
{
	float4 Position : POSITION;
	float2 Tex : TEXCOORD0;
};

struct PixelInputType
{
    float4 Position : SV_POSITION;
    float2 Tex : TEXCOORD0;
};

PixelInputType MultiTextureVertexShader(VertexInputType Input)
{
    PixelInputType Output = (PixelInputType)0;
	
    Input.Position.w = 1.0f;
	
    Output.Position = mul(Input.Position, WorldMatrix);
    Output.Position = mul(Output.Position, ViewMatrix);
    Output.Position = mul(Output.Position, ProjectionMatrix);
    
    Output.Tex = Input.Tex;
    
    return Output;
}
