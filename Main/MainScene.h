#pragma once
#include "Scene.h"
#include "OgreTerrain.h"


class StarRepresentation;
class Star;
class Boat;

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


	Ogre::TexturePtr _fogTexture;
	Ogre::TextureUnitState* _fogTextureLink;
	Ogre::MaterialPtr _fogMaterial;
	Ogre::Entity* _fogEnt;
	Ogre::SceneNode* _fogNode;
	Ogre::GpuProgramParametersSharedPtr _fogParamPtr;
public:
	MainScene( MyApplication* app, const xml_node<>* rootNode);
	virtual ~MainScene(void);
	virtual void createCameras( Ogre::RenderWindow* win );
	virtual void createScene();
	virtual bool update( const Ogre::FrameEvent& evt ); 
	virtual void load();
	void createBoatsAndStars();
	virtual bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
	virtual bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
	void createTerrain();
	void createFogMesh();
	void createFog();

	void readPixelFromTexture(Ogre::TexturePtr texturePtr,Ogre::Vector3* output);
	void addPortLight(const Ogre::Vector3& pos);
};

