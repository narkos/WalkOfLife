#include "PlayerObject.h"
#include "CollectableObject.h"
#include "Platform.h"


class Collision{
public:

	Collision(PlayerObject player){
		this->player = player;
	}

	void TestCollision(vector<Platform> platforms, vector<CollectableObject> collectables);

private:
	//anv�nds f�r att kolla hur spelaren kommer kunna flytta sig
	PlayerObject player;

	bool canGoRight;
	bool canGoLeft;
	bool canGoUp;
	bool onGround;
	
	float yValue; //den p� marken under spelaren
};