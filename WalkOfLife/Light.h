#include "Entity.h"

class Light : public Entity{
protected:	
	float intensity;
	int lightType;
	ID3D11Buffer *lightBuffer;

	virtual void CreateLightBuffer(ID3D11Device* dev);



public:
	Light(float intensity, XMFLOAT3 pos, bool isActive, bool isStatic) : Entity(pos, isActive, isStatic){
		this->intensity = intensity;
	}
	~Light();
	// Directional Light
	struct DirLight
	{
		XMFLOAT4 ambient;
		XMFLOAT4 diffuse;
		XMFLOAT4 specular;
		XMFLOAT3 direction;
		float shaderpadding;
	};

	// Point Light
	struct PointLight
	{
		XMFLOAT4 ambient;
		XMFLOAT4 diffuse;
		XMFLOAT4 specular;
		XMFLOAT3 position;
		float range;
		XMFLOAT3 attenuation;
		float shaderpadding;
	};
	// Spot Light
	struct SpotLight
	{

	};
	DirLight dLightData;
	void CreateDirLight(ID3D11Device* dev,
							XMFLOAT4 amb,
							XMFLOAT4 diff,
							XMFLOAT4 spec,
							XMFLOAT3 direction);

	void Render(ID3D11Device* dev, ID3D11DeviceContext* devcon);
	//void UpdateBuffer(ID3D11Device* dev);
	
};

//NOTES 
//		2015-04-29 Ändrade i project config -> general -> Intermediate Directory
//					från $(Platform)\$(Configuration)\
//					till $(Platform)\$(Configuration)\$(ProjectName)\
//					pga Fatal Error: C1041	