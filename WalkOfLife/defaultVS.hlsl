
//VERTEX SHADER
cbuffer World : register (c0)
{
	matrix View;
	matrix Projection;
	matrix WorldSpace;

};

cbuffer charWorld : register(c1)
{
	matrix world;
};


struct VS_IN
{
	float3 Pos : POSITION;
	float2 Tex : TEXCOORD;
	float3 normals : NORMAL;

};

struct VS_OUT
{
	float3 pos		: POSITION;
	float2 Tex		: TEXCOORD;
	float4 normals	: NORMAL;
	float4 wPos		: SV_POSITION;

};
//-----------------------------------------------------------------------------------------
// VertexShader: VSScene
//-----------------------------------------------------------------------------------------
VS_OUT VS_main(VS_IN input)
{
	VS_OUT output;

	float4 outpos = float4(input.Pos, 1.0f);

		outpos = mul(outpos, world);
		//outpos = mul(outpos, WorldSpace);

	outpos = mul(outpos, View);
	outpos = mul(outpos, Projection);

	output.pos = mul(input.Pos, WorldSpace);
	//output.pos = input.Pos;
	output.Tex = input.Tex;
	output.normals = float4(input.normals, 1.0f);
	//output.wPos = outpos;
	output.wPos = outpos;
	return output;
};
