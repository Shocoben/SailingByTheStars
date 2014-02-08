#pragma once
#include "Ogre.h"
#include "OIS.h"

using namespace Ogre;
class Scene;
class MyApplication;

class ApplicationListener : public Ogre::FrameListener
{
protected :
	OIS::InputManager* _inputManager;
	OIS::Keyboard* _keyboard;
	OIS::Mouse* _mouse;
	std::vector<Scene*> _scenes;
	Scene* _currentScene;
	MyApplication* _application;
	bool leftButtonDown;

public :
	ApplicationListener( MyApplication* app);
	void setMouseViewportSize(Ogre::Viewport* viewPort);
	~ApplicationListener();

	OIS::Keyboard* keyboard()
	{
		return _keyboard;
	}

	OIS::Mouse* mouse()
	{
		return _mouse;
	}


	int getLastSceneID();

	virtual bool frameStarted(const Ogre::FrameEvent& evt);
	bool frameEnded(const Ogre::FrameEvent& evt);
	bool frameRenderingQueued(const Ogre::FrameEvent& evt);
	void addScene(Scene* scene);
	void loadScene(Scene* scene);
	void loadScene(int sceneID);
	

};