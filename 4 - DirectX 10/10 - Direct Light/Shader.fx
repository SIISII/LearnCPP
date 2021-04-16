matrix  World;
matrix  View;
matrix  Projection;

float4  Light_Color;
float4  Light_Direction;

struct  VS_INPUT
{
    float4  Pos    : POSITION;
    float3  Normal : NORMAL;
};


struct  PS_INPUT
{
    float4  Pos    : SV_POSITION;
    float3  Normal : TEXTCOORD0;
};


PS_INPUT  VS(VS_INPUT  Data)
{
    PS_INPUT  Out;

    Out.Pos    = mul(Data.Pos, World);
    Out.Pos    = mul(Out.Pos, View);
    Out.Pos    = mul(Out.Pos, Projection);
    Out.Normal = mul(Data.Normal, World);

    return Out;
}


float4  PS_Direct_Light(PS_INPUT  Data) : SV_Target
{
    float4  Color = Light_Color * dot( (float3) Light_Direction, Data.Normal);

    float4  Final_Color = saturate(Color);

    return Final_Color;
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
        SetPixelShader(CompileShader(ps_4_1, PS_Direct_Light()));
        SetRasterizerState(RS_No_Culling);
    }
}