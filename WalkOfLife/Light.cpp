#include "Light.h"



LightClass::LightClass(XMFLOAT3 pos, bool isActive, bool isStatic) : Entity(pos, isActive, isStatic)
{

	lightObject.Position = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	lightObject.Color(1.0f, 1.0f, 1.0f, 1.0f);
	lightObject.Type(l_Directional);
	lightObject.Active(1);
	lightObject.Direction(0.0f, 0.0f, 1.0f, 0.0f);
	lightObject.SpotConeAngle(XM_PIDIV2);
	lightObject.AttConst(1.0f);
	lightObject.AttLinear(0.0f);
	lightObject.AttQuadratic(0.0f);
	


}

void LightClass::ToggleActive()
{
	if (lightObject.Active != 0)
		lightObject.Active = 0;
	else
		lightObject.Active = 1;

	return;
}
void LightClass::CreateLightBuffer(ID3D11Device* dev)
{

	D3D11_BUFFER_DESC lbuffDesc;
	ZeroMemory(&lbuffDesc, sizeof(lbuffDesc));
	lbuffDesc.Usage = D3D11_USAGE_DEFAULT;
	lbuffDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lbuffDesc.CPUAccessFlags = 0;
	lbuffDesc.MiscFlags = 0;
	lbuffDesc.ByteWidth = sizeof(Light);
	
	HRESULT hr = dev->CreateBuffer(&lbuffDesc, NULL, &lightBuffer);
	
}


void LightClass::Render(ID3D11Device* dev, ID3D11DeviceContext* devcon)
{

	//if (lightType == 1)
	//	devcon->UpdateSubresource(lightBuffer, 0, NULL, &dLightData, 0, 0);
	//else if (lightType == 2)
	//	devcon->UpdateSubresource(lightBuffer, 0, NULL, &dLightData, 0, 0);
	//else if (lightType == 3)
	//	devcon->UpdateSubresource(lightBuffer, 0, NULL, &dLightData, 0, 0);
	//else
	//	return;

	//if (lightType != 0)
	//	devcon->PSSetConstantBuffers(0, 1, &lightBuffer);

}

LightClass::~LightClass()
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