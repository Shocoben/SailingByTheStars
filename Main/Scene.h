#pragma once
#include "Ogre.h"
#include "OIS.h"


class Scene;
class GameObject;
class ApplicationListener;
class MyApplication;

using namespace Ogre;

class Scene : public OIS::MouseListener, public OIS::KeyListener
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
	Camera* getMainCamera();
	ApplicationListener* getAppListener();

	Ogre::SceneManager* sceneManager();

protected :
	Camera* _mainCamera;
	Viewport* _viewport;
	
	MyApplication* _application;
	ApplicationListener* _appListener;
	Ogre::SceneManager* _sceneManager;
	
	std::vector<GameObject*> _gameObjectsList;
	int _listenerID;

	virtual bool mouseMoved (const OIS::MouseEvent &arg);
	virtual bool mousePressed (const OIS::MouseEvent &arg, OIS::MouseButtonID id);
	virtual bool mouseReleased (const OIS::MouseEvent &arg, OIS::MouseButtonID id);
	virtual bool keyPressed (const OIS::KeyEvent &arg);
	virtual bool keyReleased (const OIS::KeyEvent &arg);

};

