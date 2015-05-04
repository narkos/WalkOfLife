#include "Collision.h"

void Collision::TestCollision(vector<Platform> platforms){


	bool canGoRight = true;
	bool canGoLeft = true;
	bool canGoUp = true;

	bool onGround = false;	

	for each (Platform p in platforms)
	{
		if (player.TestRight(p, 1) == true){ //den träffa nått till höger
			canGoRight = false;
		}
		if (player.TestLeft(p, 1) == true){ //borde nog vara mindre än 10, men testa!
			canGoLeft = false;
		}
		if (player.TestUp(p, 1) == true){ 
			canGoUp = false;
		}

		if (player.TestDown(p, 10) == true){
			//testa först mot triangeln
			groundPlatform = p; //sen använder man denna för att kolla vilket y värde
			onGround = true;
		}
		
	}
}

float Collision::GetYValueOnGround(){
	float yValue;
	XMVECTOR vecDown;
	vecDown = XMVectorSet(0, -1, 0, 0);
	yValue = player.GetYValueOnMesh(groundPlatform, vecDown, 10);

	return yValue;
}