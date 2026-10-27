Texture2D Texture : register(t0, space2);
SamplerState Sampler : register(s0, space2);

struct Input
{
    float4 colour : COLOR0;
    float2 uv : TEXCOORD0;
};

struct Output
{
    float4 color : SV_Target0;
};

Output main(Input input)
{
    Output output;
    
    float4 textureColour = Texture.Sample(Sampler, input.uv);
    
    output.color = textureColour;
    
    return output;
}