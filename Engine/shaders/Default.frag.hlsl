struct Input
{
    float4 colour : COLOR0;
};

struct Output
{
    float4 color : SV_Target0;
};

Output main(Input input)
{
    Output output;
    output.color = input.colour;
    
    return output;
}