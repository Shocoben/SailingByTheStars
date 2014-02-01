#pragma once
#include "scene.h"
class MainScene :public Scene
{

protected :
	Ogre::Sphere* _skySphere;
	GameObject* _movingObject;
	int _nbrStars;
	GameObject** _stars;

	float _movementSpeed;

public:
	MainScene(MyApplication* app);
	virtual ~MainScene(void);
	virtual void createCameras(Ogre::RenderWindow* win);
	virtual void createScene();
	virtual bool update(const Ogre::FrameEvent& evt); 
	virtual void load();
	void createStars();


};

