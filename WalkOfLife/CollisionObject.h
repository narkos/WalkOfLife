#pragma once

#include "GameObject.h"

class CollisionObject : public GameObject{
protected:
	//dessa tv� modifieras automatiskt av subklasserna: CollectableObject och Platform
	bool isCollectable; //anv�nds n�r spelaren kollar collision mot detta objekt, ifall denna �r sann s� �r collidern en CollectableObject
	bool isDeadly; //d�r man om man colliderar med detta objekt?
	
	BoundingOrientedBox bbox;

public:
	CollisionObject(ID3D11Buffer *b, XMFLOAT3 pos, bool isActive, bool isStatic, BoundingOrientedBox bbox) : GameObject(b, pos, isActive, isStatic){ //skicka sedan in BBOX:en i konstruktorn!
		this->bbox = bbox;
	}

	~CollisionObject(){

	}

	void UpdateBBOX(){ //ifall man flyttar runt detta object s� m�ste ox� BBOXen updateras!
		//bbox.Transform(bbox, world);
	} 

	void CalculateWorld(){ //denna ska kallas innan man skickar in den i GPUn
		if (isActive == true && isStatic == false){ //kanske b�ttre att check isStatic i main koden ist�llet
			world = scale * rot * pos; //st�mmer detta?
			UpdateBBOX();
		}
	}
	//sedan b�r vi kanske ha en funktion GetIntersects() som h�mtar alla object som denne collidat med


//	virtual bool isCollectable(){ return false; } //s� beh�ver man bara vara specifik med att det �r ett CollisionObject n�r man testar
//	virtual bool isDeadly(){ return false; }

	BoundingOrientedBox GetBBOX(){ return bbox; }

};