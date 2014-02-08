#pragma once
#include "Ogre.h"
//#include "Collider.h"
class BaseCollider;
class Scene;

using namespace Ogre;
class GameObject
{
public :
	GameObject();
	GameObject(Scene* myScene);
	GameObject(Scene* myScene, const String &name);

	const BaseCollider* getCollider();
	void setCollider(BaseCollider* collider);

	void objectHaveBeenErasedFromlist(const unsigned int objSceneID);
	unsigned int getSceneID();

	virtual void update(const Ogre::FrameEvent& evt);

	SceneNode* getNode();
	SceneNode* getNode() const;
	~GameObject(void);
protected :
	Scene* _myScene;
	unsigned int _sceneID;
	SceneNode* _node;
	BaseCollider* _collider;
};


