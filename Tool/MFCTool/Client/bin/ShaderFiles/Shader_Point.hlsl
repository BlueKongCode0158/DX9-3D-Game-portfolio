
matrix		g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
bool		g_isCollision = false;

struct VS_IN
{
	float3		vPosition : POSITION;
};

struct VS_OUT
{

	float4		vPosition : POSITION;
};


VS_OUT VS_MAIN(/* Á¤Á¡ */VS_IN In)
{
	VS_OUT			Out;

	matrix			matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(float4(In.vPosition, 1.f), matWVP);

	return Out;
}



struct PS_IN
{
	float4		vPosition : POSITION;
};

struct PS_OUT
{

	float4		vColor : COLOR0;
};


PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT			Out;

	Out.vColor = vector(0.f, 1.f , 0.f, 1.f);

	return Out;
}

PS_OUT PS_MAIN_NONE_SELECT(PS_IN In)
{
	PS_OUT			Out;

	Out.vColor = vector(0.f, 0.f, 1.f, 1.f);

	return Out;
}

PS_OUT	PS_MAIN_SELECT(PS_IN In)
{
	PS_OUT			Out;

	Out.vColor = vector(1.f, 0.f, 0.f, 1.f);

	return Out;
}

technique DefaultTechnique
{
	pass DefaultRendering
	{
		//FillMode = wireframe;

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN();
	}
	pass DefaultRendering
	{
		//FillMode = wireframe;

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN_NONE_SELECT();
	}
	pass DefaultRendering
	{
		//FillMode = wireframe;

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN_SELECT();
	}

}

