#pragma once

#include "Entity.h"

struct MatInfo
{
	MatInfo()
		: Emissive(0.0f, 0.0f, 0.0f, 1.0f),
		Ambient(0.1f, 0.1f, 0.1f, 1.0f),
		Diffuse(1.0f, 1.0f, 1.0f, 1.0f),
		Specular(1.0f, 1.0f, 1.0f, 1.0f),
		SpecPow(128.0f),
		UseTexture(false){}

	XMFLOAT4 Emissive;
	XMFLOAT4 Ambient;
	XMFLOAT4 Diffuse;
	XMFLOAT4 Specular;
	float SpecPow;
	bool UseTexture;
	float Padding[2];


};

struct MaterialProperties
{
	MatInfo Material;
};

class GameObject : public Entity{
public:
	GameObject(ID3D11Buffer *b, XMFLOAT3 pos, bool isActive, bool isStatic) : Entity(pos, isActive, isStatic){
		this->vertexBuffer = b;
	}

	~GameObject(){
		//vertexBuffer->Release();
		//indexBuffer->Release();
	}

	ID3D11Buffer* GetVertexBuffer(){
		return vertexBuffer;
	}

	ID3D11Buffer* GetIndexBuffer(){
		return indexBuffer;
	}

	int nrElements;

	ID3D11Buffer *vertexBuffer;
	ID3D11Buffer *indexBuffer;
protected:
	struct Triangle{
		XMVECTOR x, y, z;
	};
	//texture
	
};