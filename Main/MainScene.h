#pragma once
#include "Scene.h"
#include "OgreTerrain.h"
#include "RapidXML\rapidxml.hpp"

class StarRepresentation;
class Star;
class Boat;
using namespace rapidxml;

class MainScene :public Scene
{

protected :
	Ogre::Sphere* _skySphere;
	StarRepresentation* _movingStar;
	int _nbrStars;
	Star** _stars;
	float _movementSpeed;
	Plane* _waterPlane;
	Plane* _waterPhysicPlane;
	Plane* _skyPlane;

	Boat** _boats;
	Ogre::Light* _sun;
	Ogre::Terrain* _terrain;
	xml_document<> _doc;
	xml_node<>* _rootNode;
	char* _xmlChar;


	Ogre::Entity* _fogEnt;
	Ogre::Plane* _fogPlane;
	Ogre::SceneNode* _fogNode;
public:
	MainScene( MyApplication* app, const char* configFile);
	virtual ~MainScene(void);
	virtual void createCameras( Ogre::RenderWindow* win );
	virtual void createScene();
	virtual bool update( const Ogre::FrameEvent& evt ); 
	virtual void load();
	void createBoatsAndStars();
	virtual bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
	virtual bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
	void createTerrain();
	void addPortLight(const Ogre::Vector3& pos);
};

