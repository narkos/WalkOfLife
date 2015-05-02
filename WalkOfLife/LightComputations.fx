#define MAX_LIGHTS 8

#define L_DIRECTIONAL 1
#define L_POINT 2
#define L_SPOT 3


struct Light
{
	float4	Position;
	float4	Color;
	int		Type;
	int		Active;
	int2	pad;
	float4	Direction;
	float	SpotConeAngle;
	float	AttConst;
	float	AttLinear;
	float	AttQuadratic;
};

struct MatInfo
{
	float4	Emissive;
	float4	Ambient;
	float4	Diffuse;
	float4	Specular;
	float	SpecPow;
	bool	UseTexture;
	float2	Padding;
};

cbuffer MaterialProperties : register(b0)
{
	Materials Material;
}

struct LightingResult
{
	float4 Diffuse;
	float4 Specular;
};

cbuffer LightProperties : register(b1)
{
	float4 CamPosition;
	float4 GlobalAmbient;

	Light lights[MAX_LIGHTS];
}

// Variable and Parameter Abbreviations Glossary
//		L = Light Vector
//		N = Surface Normal
//		V = Point to Eye Vector
//		P = Point in World Space



float4 calcDiffuse(Light light, float3 L, float3 N)
{
	float diffCalc;
	diffCalc = max(0, dot(L, N));	//Returns 0 if surface isn't facing camera.
	return light.Color * diffCalc;
}

float4 calcSpecular(Light light, float3 V, float3 L, float3 N)
{
	//Phong
	float3 R = normalize(reflect(-L, N));
	float RdotV = max(0, dot(N, H));

	// Blin Phong
	float3 H = normalize(L + V);
	float NdotH = max(0, dot(N, H));

	return light.Color * pow(RdotV, Material.SpecPow);
	
}

float calcAtt(Light light, float distance)
{

}


LightingResult createPointLight(Light light, float3 V, float4 P, float3 N)
{

}

LightingResult createDirectional(Light, light, float3 V, float4 P, float3 N)
{

}

float calcSpotCone(Light light, float3 L)
{

}

LightingResult createSpotLight(Light light, float3 V, float4 P, float3 N)
{

}

LightingResult ComputeLighting(float4 P, float3 N)
{

}