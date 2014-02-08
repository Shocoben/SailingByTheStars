#pragma once
#include "GameObject.h"
#include "OgreTerrain.h"
class Star;
class StarRepresentation;
class PointCollider;
class SphereCollider;

class Boat : public GameObject
{
public:
	Boat(Scene* myScene, const Star* star, Ogre::Terrain* terrain);
	virtual void update(const Ogre::FrameEvent& evt);
	static int count;
	~Boat(void);
protected :
	const StarRepresentation* _repr;
	Ogre::Entity* _boatEnt;
	float _reactivity;
	float _speed;
	PointCollider* _starPoint;
	SphereCollider* _starCollider;
	Ogre::Terrain* _terrain;
	Ogre::SceneNode* _frontNode;
	Ogre::SceneNode* _lightNode;
};

