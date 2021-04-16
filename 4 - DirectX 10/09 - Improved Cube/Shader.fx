matrix  World;
matrix  View;
matrix  Projection;


struct  VS_INPUT
{
    float4  Pos   : POSITION;
    float4  Color : COLOR;
};


struct  PS_INPUT
{
    float4  Pos   : SV_POSITION;
    float4  Color : COLOR;
};


PS_INPUT  VS(VS_INPUT  Data)
{
    PS_INPUT  Out;

    Out.Pos   = mul(Data.Pos, World);
    Out.Pos   = mul(Out.Pos, View);
    Out.Pos   = mul(Out.Pos, Projection);

    Out.Color = Data.Color;

    return Out;
}


float4  PS(PS_INPUT  Data) : SV_Target
{
    return Data.Color;
}

RasterizerState  RS_No_Culling
{
    CullMode = Back;
};

technique10  Render_White
{
    pass  p0
    {
        SetVertexShader(CompileShader(vs_4_1, VS()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_4_1, PS()));
        SetRasterizerState(RS_No_Culling);
    }
}