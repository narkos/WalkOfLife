#include "Physics.h"


Physics::Physics()
{
	this->downforce = 0.01;
	this->upforce = 0;
	this->onPlatform = false;
}


Physics::~Physics()
{
}

void Physics::Gravitation(PlayerObject *theCharacter)
{
	if (theCharacter->jumping == true)
	{
		this->upforce = this->upforce * 1.30;
	}

	if (this->upforce >0.8)
	{
		theCharacter->jumping = false;
	}
	if (theCharacter->yPos <= -4)
	{
		this->onPlatform = true;
		this->downforce = 0.1;
	}
	if (theCharacter->yPos >-4 || this->upforce > 0)
	{
		if (theCharacter->yPos > -4)
		{
			this->downforce = this->downforce * 1.02;
		}
		theCharacter->Translate(0.0, (this->upforce - this->downforce)/2, 0.0);
		theCharacter->yPos += (this->upforce - this->downforce)/2;
	}

	if (this->upforce > 0)
	{
		this->upforce -= this->downforce / 9;
	}
}

void Physics::Jump(PlayerObject *theCharacter)
{
	this->upforce = 0.1;
	theCharacter->jumping = true;
}