struct  VS_Input
{
    float3  position_local : POS;
};


struct  VS_Output
{
    float4  position_clip : SV_POSITION;
};


VS_Output  Vertex_Shader(VS_Input  Input)
{
  VS_Output  Output = (VS_Output) 0;

  Output.position_clip = float4(Input.position_local, 1.0);

  return Output;
}


float4  Pixel_Shader(VS_Output  Input) : SV_TARGET
{
  return float4(1.0, 1.0, 0.5, 1.0);
}
