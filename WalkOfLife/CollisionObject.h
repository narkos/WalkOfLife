#pragma once

#include "GameObject.h"

class CollisionObject : public GameObject{
protected:
	//dessa två modifieras automatiskt av subklasserna: CollectableObject och Platform
	bool isCollectable; //används när spelaren kollar collision mot detta objekt, ifall denna är sann så är collidern en CollectableObject
	bool isDeadly; //dör man om man colliderar med detta objekt?
	
	BoundingOrientedBox bbox;

public:
	CollisionObject(ID3D11Buffer *b, XMFLOAT3 pos, bool isActive, bool isStatic, BoundingOrientedBox bbox) : GameObject(b, pos, isActive, isStatic){ //skicka sedan in BBOX:en i konstruktorn!
		this->bbox = bbox;
	}

	CollisionObject(){}
	~CollisionObject(){

	}

	void UpdateBBOX(){ //ifall man flyttar runt detta object så måste oxå BBOXen updateras!
		//bbox.Transform(bbox, world);
	} 

	void CalculateWorld(){ //denna ska kallas innan man skickar in den i GPUn
		if (isActive == true && isStatic == false){ //kanske bättre att check isStatic i main koden istället
			world = scale * rot * pos; //stämmer detta?
			UpdateBBOX();
		}
	}
	//sedan bör vi kanske ha en funktion GetIntersects() som hämtar alla object som denne collidat med


//	virtual bool isCollectable(){ return false; } //så behöver man bara vara specifik med att det är ett CollisionObject när man testar
//	virtual bool isDeadly(){ return false; }

	BoundingOrientedBox GetBBOX(){ return bbox; }

};