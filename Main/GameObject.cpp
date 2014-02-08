#include "GameObject.h"
#include "Scene.h"
#include "Collider.h"

GameObject::GameObject()
{

}

void GameObject::update(const Ogre::FrameEvent& evt)
{
	
}

GameObject::GameObject(Scene* myScene) : _node(myScene->sceneManager()->getRootSceneNode()->createChildSceneNode() ), _myScene(myScene), _collider( new SphereCollider( Vector3( 0,0,0 ), _node, 3 ) )
{
	_myScene->addToScene(this);
}

GameObject::GameObject(Scene* myScene, const String &name) : _node(myScene->sceneManager()->getRootSceneNode()->createChildSceneNode(name) ), _myScene(myScene), _collider( new SphereCollider( Vector3( 0,0,0 ), _node, 3 ) )
{
	_myScene->addToScene(this);
}

SceneNode* GameObject::getNode()
{
	return _node;
}


SceneNode* GameObject::getNode() const
{
	return _node;
}

void GameObject::setCollider(BaseCollider* collider)
{
	delete _collider;
	_collider = collider;
}

unsigned int GameObject::getSceneID()
{
	return _sceneID;
}

const BaseCollider* GameObject::getCollider()
{
	return _collider;
}

void GameObject::objectHaveBeenErasedFromlist(const unsigned int objSceneID)
{
	if (objSceneID < _sceneID)
		--_sceneID;
}


GameObject::~GameObject(void)
{
	if (_myScene != NULL)
		_myScene->removeFromScene(this);
	delete _collider;
}


