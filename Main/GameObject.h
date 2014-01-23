#pragma once
#include "Ogre.h"
//#include "Collider.h"
class Collider;;
class Scene;


class GameObject
{
private :
	Scene* _myScene;
	Ogre::Node* _node;
	Collider* _collider;
	unsigned int _sceneID;
	std::string _name;
public :
	GameObject(Scene* scene, Ogre::Node* node);
	GameObject(Scene* scene, Ogre::Node* node, const char* name);
	const Collider* getCollider();
	void setCollider(Collider* collider);
	void objectHaveBeenErasedFromlist(const unsigned int objSceneID);
	unsigned int getSceneID();
	
	const Ogre::Node* getNode();
	const Ogre::Node* getNode() const;

	const std::string* getName();
	void setName(std::string& name);
	~GameObject(void);
};


