matrix  g_WorldMatrix, g_ViewMatrix, g_ProjectionMatrix;
texture g_DiffuseTexture;

sampler DiffuseSampler = sampler_state
{
	texture = g_DiffuseTexture;
	mipfilter = linear;
	magfilter = linear;
};

struct VS_IN
{
	float3 vPosition : POSITION;
	float3 vTexUV	 : TEXCOORD0;
};
struct VS_OUT
{
	float4 vPosition : POSITION;
	float3 vTexUV	 : TEXCOORD0;
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT Out = (VS_OUT)0;

	Out.vPosition = mul(vector(In.vPosition,1.f), g_WorldMatrix);
	Out.vPosition = mul(Out.vPosition, g_ViewMatrix);
	Out.vPosition = mul(Out.vPosition, g_ProjectionMatrix);
	Out.vTexUV = In.vTexUV;

	return Out;
}

struct PS_IN
{
	float4 vPosition : POSITION;
	float3 vTexUV	 : TEXCOORD0;
};
struct PS_OUT
{
	float4 vColor	 : COLOR0;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;
	
	Out.vColor = texCUBE(DiffuseSampler, In.vTexUV);

	return Out;
}

technique DefaultTechnique
{
	pass DefaultRendering
	{
		Cullmode = CW;
		ZWRITEENABLE = false;
		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN();
	}
};