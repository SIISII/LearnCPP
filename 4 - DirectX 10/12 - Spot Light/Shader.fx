matrix  World;
matrix  View;
matrix  Projection;

float4  Light_Color;
float4  Light_Position;
float4  Light_Direction;
float   Light_Phi;
float   Light_Theta;


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


float  Get_Angle_Intensity(
    float3  Light_Direction,
    float3  Pixel_To_Light)
{
    float  Angle_Intensity;

    float  Cos_Alpha = saturate(dot(Light_Direction, -Pixel_To_Light));

    if ( Cos_Alpha > cos(Light_Theta / 2) )
    {
        Angle_Intensity = 1.0;
    }
    else if ( Cos_Alpha < cos(Light_Phi / 2) )
    {
        Angle_Intensity = 0.0;
    }
    else
    {
        Angle_Intensity = saturate( 
            pow(
                (Cos_Alpha - cos(Light_Phi / 2))
                /
                (cos(Light_Theta / 2) - cos(Light_Phi / 2)),
                1.0) );
    }

    return Angle_Intensity;
}


float4  PS_Spot_Light(PS_INPUT  Data) : SV_Target
{
    float  A = 0.2;
    float  B = 0.0;
    float  C = 0.0;

    float3  Pixel_To_Light = (float3) Light_Position - Data.World_Pos;

    float  Dist = length(Pixel_To_Light);

    float3  New_Direction = normalize(Pixel_To_Light);

    float3  New_Normal = normalize(Data.Normal);

    float  Light_Attenuation = 1 / (A * Dist * Dist + B * Dist + C);

    float  Light_Angle = Get_Angle_Intensity(normalize((float3) Light_Direction), New_Direction);

    float4  Light_Intensity = Light_Color * Light_Angle * Light_Attenuation;

    float4  Color = 0.2 + max(Light_Intensity * dot(New_Normal, New_Direction), 0);

    float4  Final_Color = saturate(Color);

    Final_Color.a = 1;

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
        SetPixelShader(CompileShader(ps_4_1, PS_Spot_Light()));
        SetRasterizerState(RS_No_Culling);
    }
}