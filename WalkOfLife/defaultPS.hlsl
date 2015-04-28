//PIXEL SHADER
Texture2D txDiffuse : register(t0);

SamplerState sampAni
{
	Filter = ANISOTROPIC;
	MaxAnisotropy = 4;
	AdressU = WRAP;
	AdressV = WRAP;
};




struct VS_OUT
{
	float3 Pos		: POSITION;
	float2 Tex		: TEXCOORD;
	float4 tunormal : NORMAL;
	float4 wPos		: SV_POSITION;
};

float4 PS_main(VS_OUT input) : SV_Target

{

	float4 Texdiffuse = txDiffuse.Sample(sampAni, input.Tex);

		return Texdiffuse;
		//return float4(1.0f,0.0f,0.0f,0.0f);

};