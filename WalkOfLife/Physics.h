#pragma once
#include "PlayerObject.h"
#include "Platform.h"
#include "CollectableObject.h"
class Physics
{
public:
	Physics();
	~Physics();
	void Gravitation(PlayerObject*);
	void Jump(PlayerObject*);
	bool onPlatform;

protected:
	float downforce;
	float upforce;
	
};

