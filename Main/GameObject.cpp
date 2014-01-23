#include "GameObject.h"
#include "Scene.h"
#include "Collider.h"

GameObject::GameObject(Scene* scene, Ogre::Node* node) : _myScene(scene), _node(node), _collider(new PointCollider(new Vector3(0,0,0), _node ), _sceneID(_myScene->getLastGOListLength())
{
	std::ostringstream tempStream;
	tempStream << "GameObject" << _sceneID;
	_name = tempStream.str();
	_myScene->addToScene(this);
}

GameObject::GameObject(Scene* scene, Ogre::Node* node, const char* name) : _myScene(scene), _node(node), _collider(new Collider(this)), _sceneID(_myScene->getLastGOListLength()), _name(name)
{
	_myScene->addToScene(this);
}

const Ogre::Node* GameObject::getNode()
{
	return _node;
}

const Ogre::Node* GameObject::getNode() const
{
	return _node;
}

void GameObject::setCollider(Collider* collider)
{
	delete _collider;
	_collider = collider;
}

unsigned int GameObject::getSceneID()
{
	return _sceneID;
}

const Collider* GameObject::getCollider()
{
	return _collider;
}

void GameObject::objectHaveBeenErasedFromlist(const unsigned int objSceneID)
{
	if (objSceneID < _sceneID)
		--_sceneID;
}

void GameObject::setName(std::string& name)
{
	_name = name;
}

const std::string* GameObject::getName()
{
	return &_name;
}



GameObject::~GameObject(void)
{
	if (_myScene != NULL)
		_myScene->removeFromScene(this);
	delete _collider;
}


