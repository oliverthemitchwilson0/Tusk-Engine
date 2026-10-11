struct Input
{
    float3 position : TEXCOORD0;
    float4 colour : TEXCOORD1;
};

struct Output
{
    float4 position : SV_Position;
    float4 colour : COLOR0;
};

Output main(Input input)
{
    Output output;
    output.position = float4(input.position.x, input.position.y, input.position.z, 1.f);
    output.colour = input.colour;
    
    return output;
}