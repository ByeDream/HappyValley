//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
cbuffer cbPerObject : register(b0)
{
	float4x4 gWorldViewProj;
};

struct VS_INPUT
{
	float4 Pos : POSITION;
	float4 Color : COLOR;
};

struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float4 Color : COLOR;
};

RasterizerState WireframeRS
{
	FillMode = Wireframe;
	CullMode = Back;
	FrontCounterClockwise = false;
};

RasterizerState SolidRS
{
	FillMode = SOLID;
	CullMode = Back;
	FrontCounterClockwise = false;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT VS(VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT)0;
	output.Pos = mul(input.Pos, gWorldViewProj);
	output.Color = input.Color;
	return output;
}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS(	VS_OUTPUT input, 
			uniform bool useColor ) : SV_Target
{
	float4 defaultColor = float4(1, 1, 1, 1);
	float4 color = useColor * (input.Color - defaultColor) + defaultColor;
   return input.Color;
}

//--------------------------------------------------------------------------------------
// Techniques
//--------------------------------------------------------------------------------------
technique11 SolidTech
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetPixelShader(CompileShader(ps_5_0, PS(true)));
		SetRasterizerState(SolidRS);
	}
}

technique11 WireframeTech
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetPixelShader(CompileShader(ps_5_0, PS(false)));
		SetRasterizerState(WireframeRS);
	}
}