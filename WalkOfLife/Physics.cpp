#include "Physics.h"


Physics::Physics()
{
	this->downforce = 0.005;
	this->upforce = 0;
	this->onPlatform = false;
}


Physics::~Physics()
{
}

void Physics::Gravitation(PlayerObject *theCharacter)
{
	if (theCharacter->yPos <=-6)
	{
		this->onPlatform = true;
	}
	if (theCharacter->yPos >-6 || this->upforce < 0)
	{
		theCharacter->Translate(0.0, this->upforce - this->downforce, 0.0);
		theCharacter->yPos -= this->downforce + this->upforce;
	}
	
	if (this->upforce < 0)
	{
		this->upforce += this->downforce;
	}
}

void Physics::Jump(PlayerObject *theCharacter)
{
	//this->upforce = -0.38;
	this->upforce = -0.3;
}