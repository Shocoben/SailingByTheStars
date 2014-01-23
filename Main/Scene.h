#pragma once
#include "MyFrameListener.h"
class Scene;

class GameObject;

class Scene : public MyFrameListener
{
public :
	Scene(Ogre::RenderWindow* win, Ogre::Root* root);
	~Scene();
	virtual void createCameras(Ogre::RenderWindow* win);
	virtual void createScene();
	virtual bool frameStarted(const Ogre::FrameEvent& evt); 
	void addToScene(GameObject* object);
	void removeFromScene( GameObject* object);
	int getLastGOListLength();
protected :
	Plane* _waterPlane;
	Camera* _camera;
	Viewport* _viewport;
	
	Ogre::SceneNode* _entNode;
	Ogre::SceneNode* _camNode;
	Ogre::SceneManager* _sceneManager;
	Ogre::Sphere* _skySphere;
	std::vector<GameObject*> _gameObjectsList;
	
	float _movementSpeed;
};

