matrix  World;
matrix  View;
matrix  Projection;


struct  VS_INPUT
{
    float4  Pos : POSITION;
};


struct  PS_INPUT
{
    float4  Pos : SV_POSITION;
};


PS_INPUT  VS(VS_INPUT  Data)
{
    PS_INPUT  Out;

    Out.Pos = mul(Data.Pos, World);
    Out.Pos = mul(Out.Pos, View);
    Out.Pos = mul(Out.Pos, Projection);

    return Out;
}


float4  PS(PS_INPUT  Pos) : SV_Target
{
    return float4(1.0f, 1.0f, 1.0f, 1.0f);
}


technique10  Render_White
{
    pass  p0
    {
        SetVertexShader(CompileShader(vs_4_1, VS()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_4_1, PS()));
    }
}