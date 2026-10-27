cbuffer CameraBuffer : register(b0, space1)
{
    float4x4 view;
    float4x4 projection;
}

cbuffer TransformBuffer : register(b1, space1)
{
    float4x4 world;
};

struct Input
{
    float3 position : TEXCOORD0;
    float2 uv : TEXCOORD1;
    float4 colour : TEXCOORD2;
};

struct Output
{
    float4 position : SV_Position;
    float4 colour : COLOR0;
    float2 uv : TEXCOORD0;
};

Output main(Input input)
{
    Output output;
    
    float4 position = float4(input.position, 1.f);
    
    position = mul(world, position);
    position = mul(view, position);
    position = mul(projection, position);
    
    output.position = position;
    output.colour = input.colour;
    output.uv = input.uv;
    
    return output;
}