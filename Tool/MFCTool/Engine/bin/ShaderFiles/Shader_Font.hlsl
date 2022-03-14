matrix g_WorldMatrix, g_ViewMatrix, g_ProjectionMatrix;

struct VS_IN
{
	float3 vPosition	: POSITION;
};

struct VS_OUT
{
	float4 vPosition	: POSITION;
};

struct PS_OUT
{
	vector	vColor		: COLOR0;
};

VS_OUT	VS_MAIN(VS_IN In)
{
	VS_OUT Out = (VS_OUT)0;

	In.vPosition	= mul(float4(In.vPosition, 1.f), g_WorldMatrix);
	In.vPosition	= mul(In.vPosition, g_ViewMatrix);
	Out.vPosition	= mul(In.vPosition, g_ProjectionMatrix);

	return Out;
}

PS_OUT PS_MAIN(VS_OUT In)
{
	PS_OUT Out = (PS_OUT)0;

	Out.vColor = float4(1.f, 1.f, 1.f, 1.f);

	return Out;
}


technique DefaultTechnique
{
	pass DefaultRendering
	{
		FillMode = wireframe;

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN();
	}
}
