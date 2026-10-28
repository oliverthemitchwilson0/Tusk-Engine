Texture2D tuskTexture : register(t0, space2);
SamplerState tuskSampler : register(s0, space2);

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
    
    float4 textureColour = tuskTexture.Sample(tuskSampler, input.uv);
    
    output.color = textureColour * input.colour;
    
    return output;
}