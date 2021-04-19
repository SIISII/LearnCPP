matrix  World;
matrix  View;
matrix  Projection;

float4  Light_Color;
float4  Light_Position;

struct  VS_INPUT
{
    float4  Pos    : POSITION;
    float3  Normal : NORMAL;
};


struct  PS_INPUT
{
    float4  Pos       : SV_POSITION;
    float3  World_Pos : TEXTCOORD0;
    float3  Normal    : TEXTCOORD1;
};


PS_INPUT  VS(VS_INPUT  Data)
{
    PS_INPUT  Out;

    Out.Pos       = mul(Data.Pos, World);
    Out.World_Pos = Out.Pos;
    Out.Pos       = mul(Out.Pos, View);
    Out.Pos       = mul(Out.Pos, Projection);
    Out.Normal    = mul(Data.Normal, World);

    return Out;
}


float4  PS_Direct_Light(PS_INPUT  Data) : SV_Target
{
    float3  Pixel_To_Light = (float3) Light_Position - Data.World_Pos;

    float3  New_Direction = normalize(Pixel_To_Light);

    float3  New_Normal = normalize(Data.Normal);

    float4  Light_Intensity = Light_Color / pow(length(Pixel_To_Light), 2.0);

    float4  Color = Light_Intensity * dot(New_Normal, New_Direction);

    Color.a = 1;

    return Color;
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