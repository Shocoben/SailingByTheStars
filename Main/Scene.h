#pragma once
#include "Ogre.h"

class Scene;
class GameObject;
class ApplicationListener;
class MyApplication;

using namespace Ogre;

class Scene
{
public :
	Scene(MyApplication* app);
	~Scene();
	virtual void createCameras(Ogre::RenderWindow* win) = 0;
	virtual void createScene() = 0;
	virtual bool update(const Ogre::FrameEvent& evt) = 0; 
	virtual void load() = 0;
	void addToScene(GameObject* object);
	void removeFromScene( GameObject* object);
	int getLastGOListLength();

	Ogre::SceneManager* sceneManager();

protected :
	Plane* _waterPlane;
	Camera* _camera;
	Viewport* _viewport;
	
	MyApplication* _application;
	ApplicationListener* _appListener;
	Ogre::SceneManager* _sceneManager;
	
	std::vector<GameObject*> _gameObjectsList;
	int _listenerID;
};

