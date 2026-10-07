struct Input
{
    
};

struct Output
{
    float4 color : SV_Target0;
};

Output main(Input input)
{
    Output output;
    output.color = float4(1.f, 1.f, 1.f, 1.f);
    
    return output;
}