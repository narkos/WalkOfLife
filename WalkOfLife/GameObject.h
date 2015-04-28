#pragma once

#include "Entity.h"


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