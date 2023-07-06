matrix	g_WorldMatrix, g_ViewMatrix, g_ProjectionMatrix;
float	g_fHpPer;

texture g_DiffuseTexture;

sampler DiffuseSampler = sampler_state
{
	texture = g_DiffuseTexture;

	mipfilter = linear;
	magfilter = linear;
};


struct VS_IN
{
	float3	vPosition : POSITION;
	float2	vTexUV	  : TEXCOORD0;
};

struct VS_OUT
{
	float4	vPosition : POSITION;
	float2	vTexUV	  :	TEXCOORD0;
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT	Out;

	Out.vPosition = mul(float4(In.vPosition, 1.f), g_WorldMatrix);
	Out.vPosition = mul(Out.vPosition, g_ViewMatrix);
	Out.vPosition = mul(Out.vPosition, g_ProjectionMatrix);
	Out.vTexUV = In.vTexUV;

	return Out;
}

struct PS_IN
{
	float4 vPosition : POSITION;
	float2 vTexUV	 : TEXCOORD0;
};

struct PS_OUT
{
	float4 vColor : COLOR0;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT Out;

	Out.vColor = tex2D(DiffuseSampler, In.vTexUV);
	return Out;
}

PS_OUT PS_Monster_HP(PS_IN In)
{
	PS_OUT Out;

	Out.vColor = tex2D(DiffuseSampler, In.vTexUV);
	Out.vColor.gb = 0.f;
	Out.vColor.ra = 1.f;

	if (In.vTexUV.x >= g_fHpPer)
	{
		Out.vColor.a = 0.1f;
	}

	return Out;
}

PS_OUT PS_Monster_BACKGOUND(PS_IN In)
{
	PS_OUT Out;

	Out.vColor = tex2D(DiffuseSampler, In.vTexUV) * 0.5f;

	return Out;
}
PS_OUT PS_Monster_HP_Boss(PS_IN In)
{
	PS_OUT Out;

	Out.vColor = tex2D(DiffuseSampler, In.vTexUV);
	Out.vColor.gb = 0.f;
	Out.vColor.r = 0.5f;
	Out.vColor.a = 1.f;
	if (In.vTexUV.x >= g_fHpPer)
	{
		Out.vColor.a = 0.1f;
	}

	return Out;
}
technique DefaultTechnique
{
	pass DefaultRendering
	{
		ZWriteEnable = true;
		AlphaBlendenable = true;
		BlendOp = Add;
		SrcBlend = SRCALPHA;
		DestBlend = INVSRCALPHA;

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN();
	}

	pass DefaultRendering
	{
		ZWriteEnable = true;
		AlphaBlendenable = true;
		BlendOp = Add;
		SrcBlend = SRCALPHA;
		DestBlend = INVSRCALPHA;
		CULLMODE = None;

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_Monster_HP();
	}
	pass DefaultRendering
	{
		ZWriteEnable = true;
		AlphaBlendenable = true;
		BlendOp = Add;
		SrcBlend = SRCALPHA;
		DestBlend = INVSRCALPHA;
		CULLMODE = None;

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_Monster_BACKGOUND();
	}
	pass DefaultRendering
	{
		ZWriteEnable = true;
		AlphaBlendenable = true;
		BlendOp = Add;
		SrcBlend = SRCALPHA;
		DestBlend = INVSRCALPHA;
		CULLMODE = None;

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_Monster_HP_Boss();
	}

};