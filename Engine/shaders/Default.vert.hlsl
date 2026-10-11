struct Input
{
    float x : TEXCOORD0;
    float y : TEXCOORD1;
    float z : TEXCOORD2;
    
    float r : TEXCOORD3;
    float g : TEXCOORD4;
    float b : TEXCOORD5;
};

struct Output
{
    float4 position : SV_Position;
    float4 colour : COLOR0;
};

Output main(Input input)
{
    Output output;
    output.position = float4(input.x, input.y, input.z, 1.f);
    output.colour = float4(input.r, input.g, input.b, 1.f);
    
    return output;
}