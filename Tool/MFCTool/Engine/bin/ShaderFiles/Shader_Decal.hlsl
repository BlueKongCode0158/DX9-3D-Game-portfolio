matrix  g_WorldInvMatrix, g_ViewInvMatrix, g_ProjectionInvMatrix;
matrix	g_WorldMatrix, g_ViewMatrix, g_ProjectionMatrix;

float g_fViewFar = 0.f;

texture g_DiffuseTexture;
sampler DiffuseSampler = sampler_state
{
	texture = g_DiffuseTexture;
	mipfilter = linear;
	magfilter = linear;
};

texture		g_DepthTexture;
sampler DepthSampler = sampler_state
{
	texture = g_DepthTexture;
	mipfilter = linear;
	magfilter = linear;
};

struct VS_IN
{
	float3	vPosition : POSITION;
};


struct VS_OUT
{
	float4	vPosition : POSITION;
};

VS_OUT VS_MAIN_DECAL(VS_IN In)
{
	VS_OUT	Out = (VS_OUT)0;

	Out.vPosition = mul(float4(In.vPosition, 1.f), g_WorldMatrix);
	Out.vPosition = mul(Out.vPosition, g_ViewMatrix);
	Out.vPosition = mul(Out.vPosition, g_ProjectionMatrix);

	// 출력물 => Clip space 좌표
	return Out;
}

struct PS_IN
{
	float4	vPosition : COLOR0;
};

struct PS_OUT
{
	float4	vColor : COLOR0;
};

PS_OUT	PS_MAIN_DECAL(PS_IN In)
{
	PS_OUT	Out = (PS_OUT)0;

	float2	vTexUV = (float2)0.f;
	vTexUV.x = (In.vPosition.x / In.vPosition.w) * 0.5f + 0.5f;
	vTexUV.y = (In.vPosition.x / In.vPosition.w) * -0.5f + 0.5f;

	vector vDepth = tex2D(DepthSampler, vTexUV);
	float  fViewZ = vDepth.x * 300.f;

	vector vProjPos = (vector)0.f;

	vProjPos.x = (vTexUV.x * 2.f - 1.f) * fViewZ;
	vProjPos.y = (vTexUV.y * -2.f + 1.f) * fViewZ;
	vProjPos.z = vDepth.x * fViewZ;
	vProjPos.w = fViewZ;

	//정점 변환
	vector vViewPos = mul(vProjPos, g_ProjectionInvMatrix);
	vector vWorldPos = mul(vViewPos, g_ViewInvMatrix);
	vector vLocalPos = mul(vWorldPos, g_WorldInvMatrix);

	float3	ObjectAbsPos = abs(vLocalPos.xyz);
	clip(0.5f - ObjectAbsPos);

	float2 decalUV = vLocalPos.xz + 0.5f;
	Out.vColor = tex2D(DiffuseSampler, decalUV);

	return Out;
}

technique DefaultTechnique
{
	pass DefaultRendering
	{
		VertexShader = compile vs_3_0 VS_MAIN_DECAL();
		PixelShader = compile ps_3_0 PS_MAIN_DECAL();
	}
};