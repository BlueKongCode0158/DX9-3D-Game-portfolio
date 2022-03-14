matrix		g_WorldMatrix, g_ViewMatrix, g_ProjectionMatrix;
texture		g_RedDiffuseTexture;

sampler RedDiffuseTexture = sampler_state
{
	texture = g_RedDiffuseTexture;

	mipfilter = linear;
	magfilter = linear;

};

texture		g_GreenDiffuseTexture;

sampler GreenDiffuseTexture = sampler_state
{
	texture = g_GreenDiffuseTexture;

	mipfilter = linear;
	magfilter = linear;

};

texture		g_BlueDiffuseTexture;

sampler BlueDiffuseTexture = sampler_state
{
	texture = g_BlueDiffuseTexture;

	mipfilter = linear;
	magfilter = linear;

};

texture		g_AlphaDiffuseTexture;

sampler AlphaDiffuseTexture = sampler_state
{
	texture = g_AlphaDiffuseTexture;

	mipfilter = linear;
	magfilter = linear;
};

texture		g_FilterTexture;

sampler FilterSampler = sampler_state
{
	texture = g_FilterTexture;

	mipfilter = linear;
	magfilter = linear;
};


struct VS_IN
{
	float3		vPosition : POSITION;
	float3		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
};

struct VS_OUT
{
	float4		vPosition : POSITION;
	float2		vTexUV : TEXCOORD0;
	float4		vWorldPos : TEXCOORD1;
	float4		vProjectionPos : TEXCOORD2;
	float4		vNormal	: NORMAL;
};


VS_OUT VS_MAIN(/* Á¤Á¡ */VS_IN In)
{
	VS_OUT			Out;

	matrix			matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjectionMatrix);

	Out.vPosition = mul(float4(In.vPosition, 1.f), matWVP);
	Out.vTexUV = In.vTexUV;
	Out.vWorldPos = mul(float4(In.vPosition, 1.f), g_WorldMatrix);
	Out.vNormal = mul(vector(In.vNormal, 0.f), g_WorldMatrix);
	Out.vProjectionPos = Out.vPosition;

	return Out;
}



struct PS_IN
{
	float4		vPosition : POSITION;
	float2		vTexUV : TEXCOORD0;
	float4		vWorldPos : TEXCOORD1;
	float4		vProjectionPos : TEXCOORD2;
	float4		vNormal	: NORMAL;
};

struct PS_OUT
{
	float4		vDiffuse : COLOR0;
	float4		vNormal	 : COLOR1;
	float4		vDepth	 : COLOR2;
};



PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;

	vector	vRedDiffuse = tex2D(RedDiffuseTexture, In.vTexUV * 10.f);
	vector	vGreenDiffuse = tex2D(GreenDiffuseTexture, In.vTexUV * 10.f);
	vector	vBlueDiffuse = tex2D(BlueDiffuseTexture, In.vTexUV * 10.f);
	vector	vAlphaDiffuse = tex2D(AlphaDiffuseTexture, In.vTexUV * 10.f);

	vector	vFilter = tex2D(FilterSampler, In.vTexUV);
	vector	vDiffuse = vFilter.r * vRedDiffuse + vFilter.g*vGreenDiffuse + vFilter.b*vBlueDiffuse + vFilter.a*vAlphaDiffuse;

	Out.vDiffuse = vDiffuse;
	Out.vDiffuse.a = 1.f;

	Out.vNormal = vector(In.vNormal.xyz*0.5f + 0.5f, 1.f);
	Out.vDepth = vector(In.vProjectionPos.w / 300.f, In.vProjectionPos.z / In.vProjectionPos.w, 0.f, 0.f);

	return Out;

}

technique DefaultTechnique
{
	pass DefaultRendering
	{
		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN();
	}
}

