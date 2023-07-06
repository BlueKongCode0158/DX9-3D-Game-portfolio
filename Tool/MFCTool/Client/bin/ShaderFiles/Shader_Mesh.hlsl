matrix  g_WorldMatrix, g_ViewMatrix, g_ProjectionMatrix;
float	g_fFar = 300.f;

vector	g_MtlTexture = vector(0.f, 0.f, 0.f, 0.f);

texture g_DiffuseTexture;

sampler DiffuseSampler = sampler_state
{
	texture = g_DiffuseTexture;
	mipfilter = linear;
	magfilter = linear;
};

texture g_NormalTexture;

sampler NormalSampler = sampler_state
{
	texture = g_NormalTexture;
	mipfilter = linear;
	magfilter = linear;
};

struct VS_IN
{
	float3 vPosition	: POSITION;
	float3 vNormal		: NORMAL;
	float3 vTangent		: TANGENT;
	float2 vTexUV		: TEXCOORD0;
};
struct VS_OUT
{
	float4 vPosition		: POSITION;
	float2 vTexUV			: TEXCOORD0;
	float3 vNormal			: NORMAL;
	float4 vProjectionPos	: TEXCOORD1;
	float3 vTangent			: TANGENT;
	float3 vBinormal		: BINORMAL;
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT Out = (VS_OUT)0;

	Out.vPosition = mul(float4(In.vPosition, 1.f), g_WorldMatrix);
	Out.vPosition = mul(Out.vPosition, g_ViewMatrix);
	Out.vPosition = mul(Out.vPosition, g_ProjectionMatrix);

	Out.vNormal		    = normalize(mul(float4(In.vNormal, 0.f), g_WorldMatrix)).xyz;
	Out.vTexUV		    = In.vTexUV;
	Out.vProjectionPos  = Out.vPosition;
	Out.vTangent		= normalize(mul(float4(In.vTangent, 0.f), g_WorldMatrix)).xyz;
	Out.vBinormal		= normalize(cross(Out.vNormal, Out.vTangent)).xyz;

	return Out;
}


struct VS_IN_LIGHT
{
	float3 vPosition	: POSITION;
	float3 vNormal		: NORMAL;
	float3 vTangent		: TANGENT;
	float2 vTexUV		: TEXCOORD0;
};

struct VS_OUT_LIGHT
{
	float4 vPosition		: POSITION;
	float2 vTexUV			: TEXCOORD0;
	float3 vNormal			: NORMAL;
	float4 vProjectionPos	: TEXCOORD1;
	float3 vTangent			: TANGENT;
	float3 vBinormal		: BINORMAL;
};

VS_OUT_LIGHT	VS_MAIN_LIGHT(VS_IN_LIGHT In)
{
	VS_OUT_LIGHT Out = (VS_OUT_LIGHT)0;

	Out.vPosition = mul(float4(In.vPosition, 1.f), g_WorldMatrix);
	Out.vPosition = mul(Out.vPosition, g_ViewMatrix);
	Out.vPosition = mul(Out.vPosition, g_ProjectionMatrix);

	Out.vNormal = normalize(mul(float4(In.vNormal, 0.f), g_WorldMatrix)).xyz;
	Out.vTexUV = In.vTexUV;
	Out.vProjectionPos = Out.vPosition;
	Out.vTangent = normalize(mul(float4(In.vTangent, 0.f), g_WorldMatrix)).xyz;
	Out.vBinormal = normalize(cross(Out.vNormal, Out.vTangent)).xyz;

	return Out;
}

struct PS_IN
{
	float4 vPosition		: POSITION;
	float2 vTexUV			: TEXCOORD0;
	float3 vNormal			: NORMAL;
	float4 vProjectionPos	: TEXCOORD1;
	float3 vTangent			: TANGENT;
	float3 vBinormal		: BINORMAL;
};
struct PS_OUT
{
	float4		vDiffuse : COLOR0;
	float4		vNormal	 : COLOR1;
	float4		vDepth	 : COLOR2;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;

	vector	vMtrIDiffuse = tex2D(DiffuseSampler, In.vTexUV);
	vector	vNormalDesc = tex2D(NormalSampler, In.vTexUV);
	float3	vNormal = vNormalDesc.xyz * 2.f - 1.f;

	Out.vDiffuse = vMtrIDiffuse;

	float3x3	TBN = float3x3(In.vTangent, In.vBinormal, In.vNormal);

	vNormal = mul(vNormal, TBN);

	Out.vNormal = vector(vNormal.xyz*0.5f + 0.5f, 1.f);
	Out.vDepth = vector(In.vProjectionPos.w / 300.f, In.vProjectionPos.z / In.vProjectionPos.w, 0.f, 0.f);

	return Out;
}

PS_OUT PS_MAIN_ALPHA(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;

	vector	vMtrIDiffuse = tex2D(DiffuseSampler, In.vTexUV);
	vector	vNormalDesc = tex2D(NormalSampler, In.vTexUV);
	float3	vNormal = vNormalDesc.xyz * 2.f - 1.f;

	Out.vDiffuse = vMtrIDiffuse;
	Out.vDiffuse.a = 0.3f;

	float3x3	TBN = float3x3(In.vTangent, In.vBinormal, In.vNormal);

	vNormal = mul(vNormal, TBN);

	Out.vNormal = vector(vNormal.xyz*0.5f + 0.5f, 1.f);
	Out.vDepth = vector(In.vProjectionPos.w / 300.f, In.vProjectionPos.z / In.vProjectionPos.w, 0.f, 0.f);

	return Out;
}

struct PS_IN_LIGHT
{
	float4 vPosition		: POSITION;
	float2 vTexUV			: TEXCOORD0;
	float3 vNormal			: NORMAL;
	float4 vProjectionPos	: TEXCOORD1;
	float3 vTangent			: TANGENT;
	float3 vBinormal		: BINORMAL;
};

struct PS_OUT_LIGHT
{
	float4		vDiffuse : COLOR0;
	float4		vNormal	 : COLOR1;
	float4		vDepth	 : COLOR2;
};


PS_OUT_LIGHT PS_MAIN_LIGHT(PS_IN_LIGHT In)
{
	PS_OUT_LIGHT Out = (PS_OUT_LIGHT)0;

	vector vMtrIDiffuse = tex2D(DiffuseSampler, In.vTexUV);

	Out.vDiffuse = vMtrIDiffuse;
	Out.vDiffuse.a = 1.f;	
	Out.vNormal = vector(In.vNormal.xyz*0.5f + 0.5f, 1.f);
	Out.vDepth = vector(In.vProjectionPos.w / 300.f, In.vProjectionPos.z / In.vProjectionPos.w, 0.f, 0.f);

	return Out;
}

PS_OUT_LIGHT PS_MAIN_LIGHT_NORMAL(PS_IN_LIGHT In)
{
	PS_OUT_LIGHT Out = (PS_OUT_LIGHT)0;

	vector vMtrIDiffuse = tex2D(DiffuseSampler, In.vTexUV) + g_MtlTexture;
	vector vNormalDesc = tex2D(NormalSampler, In.vTexUV);

	vector vNormalDeffault = vector(vNormalDesc.b, vNormalDesc.a, 1.f, 1.f);

	float3 vNormal = vNormalDeffault.xyz * 2.f - 1.f;

	Out.vDiffuse = vMtrIDiffuse;
	Out.vDiffuse.a = 1.f;

	float3x3	TBN = float3x3(In.vTangent, In.vBinormal, In.vNormal);

	vNormal = mul(vNormal, TBN);

	Out.vNormal = vector(vNormal.xyz*0.5f + 0.5f, 1.f);
	Out.vDepth = vector(In.vProjectionPos.w / 300.f, In.vProjectionPos.z / In.vProjectionPos.w, 0.f, 0.f);

	return Out;
}

PS_OUT_LIGHT PS_MAIN_LIGHT_NORMAL_DEFAULT(PS_IN_LIGHT In)
{
	PS_OUT_LIGHT Out = (PS_OUT_LIGHT)0;

	vector vMtrIDiffuse = tex2D(DiffuseSampler, In.vTexUV);
	vector vNormalDesc = tex2D(NormalSampler, In.vTexUV);

	float3 vNormal = vNormalDesc.xyz * 2.f - 1.f;

	Out.vDiffuse = vMtrIDiffuse;
	Out.vDiffuse.a = 1.f;

	float3x3	TBN = float3x3(In.vTangent, In.vBinormal, In.vNormal);

	vNormal = mul(vNormal, TBN);

	Out.vNormal = vector(vNormal.xyz*0.5f + 0.5f, 1.f);
	Out.vDepth = vector(In.vProjectionPos.w / 300.f, In.vProjectionPos.z / In.vProjectionPos.w, 0.f, 0.f);

	return Out;
}

matrix		g_matLightView;
matrix		g_matLightProjection;

struct VS_IN_SHADOW
{
	float3	vPosition : POSITION;
};

struct VS_OUT_SHADOW
{
	float4	vPosition : POSITION;
	float4	vProjectionPos : TEXCOORD0;
};

VS_OUT_SHADOW VS_MAIN_SHADOW(VS_IN_SHADOW In)
{
	VS_OUT_SHADOW	Out = (VS_OUT_SHADOW)0;

	Out.vPosition = mul(float4(In.vPosition, 1.f), g_WorldMatrix);
	Out.vPosition = mul(Out.vPosition, g_ViewMatrix);
	Out.vPosition = mul(Out.vPosition, g_ProjectionMatrix);
	Out.vProjectionPos = Out.vPosition;

	return Out;
}

struct PS_IN_SHADOW
{
	float4	vPosition: POSITION;
	float4	vProjectionPos : TEXCOORD0;
};

struct PS_OUT_SHADOW
{
	vector	vLightDepth : COLOR0;
};

PS_OUT_SHADOW PS_MAIN_SHADOW(PS_IN_SHADOW In)
{
	PS_OUT_SHADOW Out = (PS_OUT_SHADOW)0;

	Out.vLightDepth.r = In.vProjectionPos / g_fFar;
	Out.vLightDepth.a = 1.f;

	return Out;
}


technique DefaultTechnique
{
	pass DefaultRendering
	{
		ALPHABLENDENABLE = false;
		AlphaTestEnable = true;
		ZWriteEnable = true;
		AlphaFunc = Greater;
		AlphaRef = 30;


		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN();
	}
	pass DefaultRendering
	{
		ALPHABLENDENABLE = false;
		AlphaTestEnable = true;
		ZWriteEnable = true;
		AlphaFunc = Greater;
		AlphaRef = 30;


		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN();
	}
	pass DefaultRendering
	{
		ALPHABLENDENABLE = false;
		ZWriteEnable = true;
		AlphaTestEnable = false;

		VertexShader = compile vs_3_0 VS_MAIN_LIGHT();
		PixelShader = compile ps_3_0 PS_MAIN_LIGHT();
	}
	pass DefaultRendering
	{
		ALPHABLENDENABLE = false;
		ZWriteEnable = true;
		AlphaTestEnable = false;

		VertexShader = compile vs_3_0 VS_MAIN_LIGHT();
		PixelShader = compile ps_3_0 PS_MAIN_LIGHT_NORMAL();
	}
	pass DefaultRendering
	{
		ALPHABLENDENABLE = false;
		ZWriteEnable = true;
		AlphaTestEnable = false;

		VertexShader = compile vs_3_0 VS_MAIN_LIGHT();
		PixelShader = compile ps_3_0 PS_MAIN_LIGHT_NORMAL_DEFAULT();
	}
	pass DefaultRendering
	{
		ALPHABLENDENABLE = false;
		ZWriteEnable = false;

		VertexShader = compile vs_3_0 VS_MAIN_SHADOW();
		PixelShader = compile ps_3_0 PS_MAIN_SHADOW();
	}
	pass DefaultRendring
	{
		ZWriteEnable = true;
		ALPHABLENDENABLE = true;
		BlendOp = Add;
		SrcBlend = SRCALPHA;
		DestBlend = INVSRCALPHA;
		CULLMODE = None;

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN_ALPHA();
	}
};