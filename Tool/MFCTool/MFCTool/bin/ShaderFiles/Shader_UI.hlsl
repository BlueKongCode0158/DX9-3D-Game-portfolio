
matrix		g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
float		g_fLoadingPer;
float		g_fHpPer;

texture		g_DiffuseTexture;
sampler DiffuseSampler = sampler_state
{
	mipfilter = point;
	magfilter = point;

	texture = g_DiffuseTexture;
};

struct VS_IN
{
	float3		vPosition : POSITION;
	float2		vTexUV	  : TEXCOORD0;
};

struct VS_OUT
{
	float4		vPosition : POSITION;
	float2		vTexUV	  :	TEXCOORD0;
};


VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT			Out;

	matrix			matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition	= mul(float4(In.vPosition, 1.f), matWVP);
	Out.vTexUV		= In.vTexUV;
	return Out;
}



struct PS_IN
{
	float4		vPosition : POSITION;
	float2		vTexUV	  : TEXCOORD0;
};

struct PS_OUT
{
	float4		vColor : COLOR0;
};


PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;

	Out.vColor = tex2D(DiffuseSampler, In.vTexUV);

	return Out;
}

PS_OUT	PS_MAIN_LOADING(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;

	Out.vColor = tex2D(DiffuseSampler, In.vTexUV);
	if (In.vTexUV.x <= g_fLoadingPer && Out.vColor.a != 0)
	{
		Out.vColor.a = 1.f;
	}
	else if(Out.vColor.a != 0)
	{
		Out.vColor.a = 0.f;
	}

	return Out;
}

PS_OUT PS_MAIN_SELECT(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;

	Out.vColor = tex2D(DiffuseSampler, In.vTexUV);
	Out.vColor.rgb *= 0.7f;
	return Out;
}

PS_OUT PS_MAIN_PLAYER_HP(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;

	Out.vColor = tex2D(DiffuseSampler, In.vTexUV);

	if (Out.vColor.a != 0.f)
	{
		Out.vColor.rb	= 0.f;
		Out.vColor.a	=1.f;
	}
	if (g_fHpPer <= In.vTexUV.x)
	{
		Out.vColor.a	= 0.2f;
	}

	return Out;
}


technique DefaultTechnique
{
	pass DefaultRendering
	{
		AlphaTestEnable = false;
		//AlphaFunc = Greater;
		//AlphaRef = 0;

		AlphaBlendenable = true;
		BlendOp = Add;
		SrcBlend = SRCALPHA;
		DestBlend = INVSRCALPHA;

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN();
	}
	pass DefaultRendering
	{
		AlphaBlendenable = false;
		AlphaTestEnable = true;
		AlphaFunc = Greater;
		AlphaRef = 0;

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN_LOADING();
	}
	pass DefaultRendering
	{
		AlphaBlendenable = false;
		AlphaTestEnable = true;
		AlphaFunc = Greater;
		AlphaRef = 0;

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN_SELECT();
	}

	pass DefaultRendering
	{
		AlphaTestEnable = false;
		AlphaBlendenable = true;
		BlendOp = Add;
		SrcBlend = SRCALPHA;
		DestBlend = INVSRCALPHA;

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN_PLAYER_HP();
	}

}

