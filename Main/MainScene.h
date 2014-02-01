#pragma once
#include "scene.h"
class MainScene :public Scene
{
protected :
	Ogre::Sphere* _skySphere;
	GameObject* _movingObject;
	GameObject** _stars;
	int _nbrStars;
	float _movementSpeed;

public:
	MainScene(Ogre::RenderWindow* win, Ogre::Root* root);
	virtual ~MainScene(void);
	virtual void createCameras(Ogre::RenderWindow* win);
	virtual void createScene();
	virtual bool frameStarted(const Ogre::FrameEvent& evt); 
	void createStars();


};

