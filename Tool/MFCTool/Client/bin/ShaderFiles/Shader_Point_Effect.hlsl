
matrix		g_WorldMatrix, g_ViewMatrix, g_ProjectionMatrix;
float		g_fWinSizeH;

texture		g_DiffuseTexture;

sampler DiffuseSampler = sampler_state
{
	texture = g_DiffuseTexture;

	mipfilter = linear;
	magfilter = linear;

	AddressU = clamp;
	AddressV = clamp;

};

struct VS_IN
{
	float3		vPosition : POSITION;
	float		fSize : PSIZE;
	float2		vTexUV : TEXCOORD0;

	float4		vRight : TEXCOORD1;
	float4		vUp : TEXCOORD2;
	float4		vLook : TEXCOORD3;
	float4		vTranslation : TEXCOORD4;
};

struct VS_OUT
{

	float4		vPosition : POSITION;
	float		fSize : PSIZE;
	float2		vTexUV : TEXCOORD0;
};


VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT			Out;

	matrix			matVW, matVWV, matVWVP;
	matrix			VtxMatrix = float4x4(In.vRight, In.vUp, In.vLook, In.vTranslation);

	matVW = mul(VtxMatrix, g_WorldMatrix);
	matVWV = mul(matVW, g_ViewMatrix);
	matVWVP = mul(matVWV, g_ProjectionMatrix);

	Out.vPosition = mul(float4(In.vPosition, 1.f), matVWVP);
	Out.fSize = (g_fWinSizeH * In.fSize) * sqrt(1.f / pow(Out.vPosition.w, 2.f));

	Out.vTexUV = In.vTexUV;


	return Out;
}



struct PS_IN
{
	float4		vPosition : POSITION;
	float		fSize : PSIZE;
	float2		vTexUV : TEXCOORD0;
};

struct PS_OUT
{
	float4		vColor : COLOR0;
};



PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT			Out;

	Out.vColor = tex2D(DiffuseSampler, In.vTexUV);

	return Out;
}

technique DefaultTechnique
{
	pass DefaultRendering
	{
		PointSpriteEnable = true;

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN();
	}
}

