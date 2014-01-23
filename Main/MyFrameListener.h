#pragma once
#include "Ogre.h"
#include "OIS.h"

using namespace Ogre;

class MyFrameListener : public Ogre::FrameListener
{
protected :
	OIS::InputManager* _inputManager;
	OIS::Keyboard* _keyboard;
	OIS::Mouse* _mouse;


public :
	MyFrameListener(Ogre::RenderWindow* win, Ogre::Root* root);
	void setMouseViewportSize(Ogre::Viewport* viewPort);
	~MyFrameListener();

	virtual bool frameStarted(const Ogre::FrameEvent& evt);
	bool frameEnded(const Ogre::FrameEvent& evt);
	bool frameRenderingQueued(const Ogre::FrameEvent& evt);
};