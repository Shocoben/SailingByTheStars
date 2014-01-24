#pragma once
#include "Ogre.h"
//#include "Collider.h"
class BaseCollider;
class Scene;

using namespace Ogre;
class GameObject
{
protected :
	Scene* _myScene;
	BaseCollider* _collider;
	unsigned int _sceneID;

	SceneNode* _node;
	

public :
	GameObject(Scene* myScene);
	GameObject(Scene* myScene, const String &name);

	const BaseCollider* getCollider();
	void setCollider(BaseCollider* collider);

	void objectHaveBeenErasedFromlist(const unsigned int objSceneID);
	unsigned int getSceneID();

	SceneNode* getNode();


	~GameObject(void);
};


