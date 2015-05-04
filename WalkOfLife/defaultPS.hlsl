//PIXEL SHADER
#include "LightComputations.fx"
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
	float4 Pos		: SV_POSITION;
	float2 Tex		: TEXCOORD;
	float3 Nor		: NORMAL;
	float4 wPos		: POSITION;
};

float4 PS_main(VS_OUT input) : SV_Target

{
	LightingResult lightCalcs = ComputeLighting(input.wPos, normalize(input.Nor));

	input.Nor = normalize(input.Nor);
	//float3 objtoEye = normalize()
	float4 Texdiffuse = txDiffuse.Sample(sampAni, input.Tex);

		float4 ambient = GlobalAmbient;
		float4 diffuse = lightCalcs.Diffuse;
		float4 specular = lightCalcs.Specular;

		float4 finalColor = (ambient + diffuse + specular) * Texdiffuse;

		return finalColor;
		//return float4(1.0f,0.0f,0.0f,0.0f);

};


//Texture2D txDiffuse : register(t0);
//
//SamplerState sampAni
//{
//	Filter = ANISOTROPIC;
//	MaxAnisotropy = 4;
//	AdressU = WRAP;
//	AdressV = WRAP;
//};
//
//
//
//
//struct VS_OUT
//{
//	float3 Pos		: POSITION;
//	float2 Tex		: TEXCOORD;
//	float4 tunormal : NORMAL;
//	float4 wPos		: SV_POSITION;
//};
//
//float4 PS_main(VS_OUT input) : SV_Target
//
//{
//
//	float4 Texdiffuse = txDiffuse.Sample(sampAni, input.Tex);
//
//	return Texdiffuse;
//	//return float4(1.0f,0.0f,0.0f,0.0f);
//
//};