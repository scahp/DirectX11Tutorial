
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
    float3 Normal : NORMAL;
    float3 Tangent : TANGENT;
    float3 Binormal : BINORMAL;
};

struct PixelInputType
{
    float4 Position : SV_POSITION;
    float2 Tex : TEXCOORD0;
    float3 Normal : NORMAL;
    float3 Tangent : TANGENT;
    float3 Binormal : BINORMAL;
};

PixelInputType BumpMapVertexShader(VertexInputType Input)
{
    PixelInputType Output;
    
    Input.Position.w = 1.0f;
    
    Output.Position = mul(Input.Position, WorldMatrix);
    Output.Position = mul(Output.Position, ViewMatrix);
    Output.Position = mul(Output.Position, ProjectionMatrix);
    
    Output.Tex = Input.Tex;
    
    Output.Normal = mul(Input.Normal, (float3x3) WorldMatrix);
    Output.Normal = normalize(Output.Normal);
    
    Output.Tangent = mul(Input.Tangent, (float3x3) WorldMatrix);
    Output.Tangent = normalize(Output.Tangent);
    
    Output.Binormal = mul(Input.Binormal, (float3x3) WorldMatrix);
    Output.Binormal = normalize(Output.Binormal);

    return Output;
}