#include "PlayerObject.h"

void PlayerObject::Move(bool right)
{
	if (right)
	{
		
		this->Translate(this->getSpeed(), 0.0, 0.0);
		//this->Rotate(XMLoadFloat3(&XMFLOAT3(0,0,1)), this->xPos);
		//this->Scale(5, 1, 1);
		this->xPos += this->getSpeed();
	}
		
	else
	{
		this->Translate(-this->getSpeed(), 0.0, 0.0);
		this->xPos -= this->getSpeed();
	}
}

void PlayerObject::Jump()
{
	
}

float PlayerObject::getSpeed()
{
	return this->speed;
}

float PlayerObject::getJumpHeight()
{
	return this->jumpHeight;
}