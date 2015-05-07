#include "Collision.h"

void Collision::TestCollision(vector<Platform> platforms){

	this->distDown = 0;
	this->canGoRight = true;
	this->canGoLeft = true;
	this->canGoUp = true;

	this->onGround = false;	

	for each (Platform p in platforms)
	{
		if (player->TestRight(p) == true){ //den tr�ffa n�tt till h�ger
			canGoRight = false;		
			
		}
		if (player->TestLeft(p) == true){ //borde nog vara mindre �n 10, men testa!
			canGoLeft = false;
			
		}
		if (player->TestUp(p) == true){ 
			canGoUp = false;			
		}

		if (player->TestDown(p) == true){
			groundPlatform = p; //sen anv�nder man denna f�r att kolla vilket y v�rde
			this->onGround = true;
			//testa f�rst mot triangeln
			
		}
		
	}
}

float Collision::GetYValueOnGround(){
	float yValue;
	XMVECTOR vecDown;
	vecDown = XMVectorSet(0, -1, 0, 0);
	//yValue = player->GetYValueOnMesh(groundPlatform, vecDown, 10);

	return 1;
}