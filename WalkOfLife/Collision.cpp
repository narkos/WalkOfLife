#include "Collision.h"

void Collision::TestCollision(vector<Platform> platforms, vector<CollectableObject> collectables){
	for each (CollectableObject c in collectables)
	{
		//player.TestIntersect(c);
	}

	bool canGoRight = true;
	bool canGoLeft = true;
	bool canGoUp = true;

	bool onGround = false;	

	for each (Platform p in platforms)
	{
		if (player.TestRight(p, 10) == true){ //den träffa nått till höger
			//testa först mot triangeln
			canGoRight = false;
		}
		if (player.TestLeft(p, 10) == true){ 
			canGoLeft = false;
		}
		if (player.TestUp(p, 10) == true){ 
			canGoUp = false;
		}

		if (player.TestDown(p, 10) == true){ 
			onGround = true;
		}
		
	}
}