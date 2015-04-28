#include "Entity.h"

class Light : public Entity{
protected:
	float range;
	float intensity;
	ID3D11Buffer *lightBuffer;
public:
	Light(float intensity, float range, XMFLOAT3 pos, bool isActive, bool isStatic) : Entity(pos, isActive, isStatic){
		this->range = range;
		this->intensity = intensity;
	}

	~Light(){}

	virtual void CreateLightBuffer(){

	}
};