#include "Light.h"



void Light::CreateDirLight(ID3D11Device* dev,
	XMFLOAT4 amb,
	XMFLOAT4 diff,
	XMFLOAT4 spec,
	XMFLOAT3 direction)
{
	dLightData.ambient = amb;
	dLightData.diffuse = diff;
	dLightData.specular = spec;
	dLightData.direction = direction;
	dLightData.shaderpadding = 0.0f;
	this->lightType = 1;
	
	CreateLightBuffer(dev);
}

void Light::CreateLightBuffer(ID3D11Device* dev)
{

	D3D11_BUFFER_DESC lbuffDesc;
	ZeroMemory(&lbuffDesc, sizeof(lbuffDesc));
	lbuffDesc.Usage = D3D11_USAGE_DEFAULT;
	lbuffDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lbuffDesc.CPUAccessFlags = 0;
	lbuffDesc.MiscFlags = 0;
	if (lightType == 1)
		lbuffDesc.ByteWidth = sizeof(DirLight);
	else if (lightType == 2)
		lbuffDesc.ByteWidth = sizeof(PointLight);
	else if (lightType == 3)
		lbuffDesc.ByteWidth = sizeof(SpotLight);
	else
		return;

	if (lightType != 0)
		HRESULT hr = dev->CreateBuffer(&lbuffDesc, NULL, &lightBuffer);
	

}


void Light::Render(ID3D11Device* dev, ID3D11DeviceContext* devcon)
{

	if (lightType == 1)
		devcon->UpdateSubresource(lightBuffer, 0, NULL, &dLightData, 0, 0);
	else if (lightType == 2)
		devcon->UpdateSubresource(lightBuffer, 0, NULL, &dLightData, 0, 0);
	else if (lightType == 3)
		devcon->UpdateSubresource(lightBuffer, 0, NULL, &dLightData, 0, 0);
	else
		return;

	if (lightType != 0)
		devcon->PSSetConstantBuffers(0, 1, &lightBuffer);

}

Light::~Light()
{
	lightBuffer->Release();
}

//void Light::UpdateBuffer(ID3D11Device* dev)
//{ 
//	if (lightType == 1)
//	{
//
//	}
// }