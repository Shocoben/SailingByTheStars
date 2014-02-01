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
	Ogre::SceneManager* sceneManager();

protected :
	Plane* _waterPlane;
	Camera* _camera;
	Viewport* _viewport;
	
	Ogre::SceneManager* _sceneManager;

	std::vector<GameObject*> _gameObjectsList;

};

