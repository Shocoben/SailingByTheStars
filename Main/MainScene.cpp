#include "MainScene.h"
#include "MathP.h"
#include "MyApplication.h"
#include "ApplicationListener.h"
#include "Star.h"
#include "Boat.h"
#include "Collider.h"
#include "OgreTerrain.h"
#include "TerrainGeneratorB.h"

MainScene::MainScene(MyApplication* app, const char* configFile) : Scene( app), _nbrStars(1), _stars(new Star*[_nbrStars]), _movementSpeed(50.0f), _movingStar(NULL), _boats(new Boat*[_nbrStars]), _xmlChar(NULL)
{
	std::stringstream stringXML;
	std::ifstream infile;
	infile.open(configFile);

	std::string string;
	while(!infile.eof())
	{
		std::getline(infile, string);
		stringXML << string;
	}
	infile.close();

	std::cout<<stringXML.str() << std::endl;
	_xmlChar = strdup(stringXML.str().c_str());
	_doc.parse<0>(_xmlChar);
	_rootNode = _doc.first_node("LevelDescription");
}


void MainScene::load()
{
	//NEED 
	_sceneManager = _application->getRoot()->createSceneManager(Ogre::ST_GENERIC);
	createCameras( _application->getWindow() );
	createScene();
}
MainScene::~MainScene(void)
{
	_terrain->freeTemporaryResources();
	delete [] _stars;
	delete [] _boats;
	delete _xmlChar;
	Scene::~Scene();
}

void MainScene::createCameras(Ogre::RenderWindow* win)
{
	_mainCamera = _sceneManager->createCamera( "MainCamera" );
	_mainCamera->setPosition( Ogre::Vector3( 0,29,0 ) );
	_mainCamera->lookAt( 0, 15, -15 );
	_mainCamera->setNearClipDistance(5);	
	_viewport= win->addViewport(_mainCamera);
	_mainCamera->setAspectRatio( Ogre::Real( _viewport->getActualWidth() ) / Ogre::Real( _viewport->getActualHeight() ) );
	_appListener->setMouseViewportSize( _viewport );
}

void MainScene::createTerrain()
{
	Ogre::TerrainMaterialGeneratorPtr matGen;
	Ogre::TerrainMaterialGeneratorB* matGenB = new Ogre::TerrainMaterialGeneratorB();
	matGen.bind(matGenB);


	Ogre::TerrainGlobalOptions* tOptions = OGRE_NEW Ogre::TerrainGlobalOptions();
	tOptions->setMaxPixelError(0);
	tOptions->setLightMapDirection(_sun->getDerivedDirection());
	tOptions->setCompositeMapDistance(3000);
	tOptions->setCompositeMapAmbient(_sceneManager->getAmbientLight());
	tOptions->setDefaultMaterialGenerator(matGen);


	_terrain = new Ogre::Terrain(_sceneManager);

	xml_node<>* terrainNode = _rootNode->first_node("Terrain");

	Ogre::Image heightMap;
	heightMap.load(terrainNode->first_attribute("heightMap")->value(), Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

	Ogre::Terrain::ImportData imp;
	imp.inputImage = &heightMap;

	imp.terrainSize = atoi(terrainNode->first_attribute("terrainSize")->value());
	imp.worldSize = atoi(terrainNode->first_attribute("worldSize")->value());
	imp.inputScale = atoi(terrainNode->first_attribute("inputScale")->value());
	imp.minBatchSize = atoi(terrainNode->first_attribute("minBatchSize")->value());
	imp.maxBatchSize = atoi(terrainNode->first_attribute("maxBatchSize")->value());

	
	int count = 0;
	for (xml_node<>* layerNode = terrainNode->first_node("Layer"); layerNode != NULL; layerNode = layerNode->next_sibling())
	{
		count++;
	}
	imp.layerList.resize(count);


	
	count = 0;
	for (xml_node<>* layerNode = terrainNode->first_node("Layer"); layerNode != NULL; layerNode = layerNode->next_sibling())
	{
		imp.layerList[count].worldSize = atoi(layerNode->first_attribute("worldSize")->value());
		imp.layerList[count].textureNames.push_back(layerNode->first_attribute("texture")->value());
		imp.layerList[count].textureNames.push_back(layerNode->first_attribute("normal")->value());
		++count;
	}
	
	Ogre::MaterialManager::getSingleton().setDefaultTextureFiltering(Ogre::TFO_ANISOTROPIC);
	Ogre::MaterialManager::getSingleton().setDefaultAnisotropy(8);

	_terrain->prepare(imp);
	_terrain->load();
}


int portLightCount = 0;
void MainScene::addPortLight(const Ogre::Vector3& position)
{
	std::stringstream lightName;
	lightName << "portLight " << portLightCount;

	Ogre::Light* portLight = _sceneManager->createLight(lightName.str());
	portLight->setType(Ogre::Light::LT_POINT);
	portLight->setDiffuseColour(0.8,0.4,0);
	portLight->setSpecularColour(0.4,0.2,0);
	portLight->setAttenuation(800, 2.0, 0.5, 0.08);

	Ogre::BillboardSet* bSet = _sceneManager->createBillboardSet(1);
	bSet->setMaterialName("Examples/Flare");
	Billboard* b = bSet->createBillboard(0,0,0, Ogre::ColourValue(0.4,0.2,0, 0.2));
	
	Ogre::SceneNode* lightNode = _sceneManager->getRootSceneNode()->createChildSceneNode(lightName.str());
	lightNode->attachObject(bSet);
	lightNode->attachObject(portLight);
	lightNode->setPosition(position);
	lightNode->scale(0.02,0.02,0.02);

	portLightCount++;
}

void MainScene::createScene()
{
	Ogre::ColourValue fadeColour(0.03f, 0.03f, 0.08f);
	_viewport->setBackgroundColour(fadeColour);
	_sceneManager->setFog(Ogre::FOG_LINEAR, fadeColour, 0.0, 200, 300);
	_sceneManager->setSkyDome(true, "Examples/NightSkyDome", 5, 4, 100);
	//_sceneManager->setSkyBox(true, "Examples/SpaceSkyBox",100);
	

	_sceneManager->setAmbientLight(Ogre::ColourValue(0.01f,0.01f,0.01f));

	_sun = _sceneManager->createLight("sun");
	_sun->setType(Ogre::Light::LT_DIRECTIONAL);
	_sun->setDirection(1,-1, -1);
	_sun->setDiffuseColour(0.2,0.2,0.2);
	_sun->setSpecularColour(0.1,0.1,0.2);

	addPortLight(Ogre::Vector3(-91,10,-75));
	addPortLight(Ogre::Vector3(-11,10,-114));
	addPortLight(Ogre::Vector3(4,10,-58));
	addPortLight(Ogre::Vector3(-60,10,20));
	addPortLight(Ogre::Vector3(48,10,-2));
	addPortLight(Ogre::Vector3(11,10,60));
	addPortLight(Ogre::Vector3(114,10,-16));
	addPortLight(Ogre::Vector3(-67,10,-123));
	addPortLight(Ogre::Vector3(12,12,-58));


	xml_node<>* waterPlaneNode = _rootNode->first_node("Planes");

	_waterPlane = new Ogre::Plane(Ogre::Vector3::UNIT_Y, atof(waterPlaneNode->first_attribute("waterPlaneY")->value()) );
	_waterPhysicPlane = new Ogre::Plane(Ogre::Vector3::UNIT_Y, atof(waterPlaneNode->first_attribute("waterPhysicPlaneY")->value()) );
	_skySphere = new Sphere(Vector3(0,0,0), 60);
	_skyPlane = new Ogre::Plane(Ogre::Vector3:: UNIT_Y, atof(waterPlaneNode->first_attribute("skyPlaneY")->value()) );
	_fogPlane = new Ogre::Plane(Ogre::Vector3:: UNIT_Y, atof(waterPlaneNode->first_attribute("fogPlaneY")->value()) );
	

	Ogre::MeshManager::getSingleton().createPlane("WaterMesh", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,*_waterPlane, atof(waterPlaneNode->first_attribute("waterMeshW")->value()), atof(waterPlaneNode->first_attribute("waterMeshH")->value()),150,150, true, 1, 7, 5, Ogre::Vector3::UNIT_Z);
	Ogre::Entity* waterEnt = _sceneManager->createEntity("WaterEnt", "WaterMesh");
	_sceneManager->getRootSceneNode()->createChildSceneNode()->attachObject(waterEnt);
	waterEnt->setMaterialName("Ocean2_Cg");


	/*
	Ogre::MeshManager::getSingleton().createPlane("WarFogMesh", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,*_fogPlane, atof(waterPlaneNode->first_attribute("waterMeshW")->value()), atof(waterPlaneNode->first_attribute("waterMeshH")->value()),150,150, true, 1, 7, 5, Ogre::Vector3::UNIT_Z);
	_fogEnt = _sceneManager->createEntity("FogEnt", "WarFogMesh");
	_fogEnt->setMaterialName("Examples/BeachStones");
	_fogNode = _sceneManager->getRootSceneNode()->createChildSceneNode();
	_fogNode->attachObject(_fogEnt);*/

	/*
	Ogre::MeshManager::getSingleton().createPlane("SimplePlane", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,*_waterPlane, 1000,1000,1,1, true, 1, 1, 1, Ogre::Vector3::UNIT_Z);
	Ogre::Entity* groundEnt = _sceneManager->createEntity("WaterFloor", "SimplePlane");
	Ogre::SceneNode* _groundNode = _sceneManager->getRootSceneNode()->createChildSceneNode();
	_groundNode->translate(0,-8,0);
	_groundNode->attachObject(groundEnt);
	groundEnt->setMaterialName("OceanFloor");*/

	createTerrain();
	createBoatsAndStars();
}

void MainScene::createBoatsAndStars()
{
	for (int i = 0; i < _nbrStars; ++i)
	{
		_stars[i] = new Star(this, _skySphere, _waterPhysicPlane, _skyPlane);
		_boats[i] = new Boat(this, _stars[i], _terrain);
	}
}

bool MainScene::mouseReleased(const OIS::MouseEvent& arg, OIS::MouseButtonID id)
{
	if (id == OIS::MouseButtonID::MB_Left && _movingStar != NULL)
	{
		_movingStar = NULL;
	}
	return true;
}

bool MainScene::mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id)
{
	if (id == OIS::MouseButtonID::MB_Left)
	{
		float mousePosX = (float)_appListener->mouse()->getMouseState().X.abs /(float) _viewport->getActualWidth();
		float mousePosY = (float)_appListener->mouse()->getMouseState().Y.abs /(float) _viewport->getActualHeight();

		Ogre::Ray mouseRay = _mainCamera->getCameraToViewportRay(mousePosX, mousePosY);
		
		
		if ( _movingStar == NULL )
		{

			for (int i = 0; i < _nbrStars; ++i)
			{
				StarRepresentation* starToMove = _stars[i]->collideWithMouse(&mouseRay);
				if (starToMove != NULL)
				{
					_movingStar = starToMove;
					std::cout<<"COLLIDE WITH STAR" << std::endl;
					break;
				}
			}
		}
	}
	return true;

}


bool MainScene::update(const Ogre::FrameEvent& evt)
{
#pragma region cameraControls

	if (_appListener->keyboard()->isKeyDown(OIS::KC_F4) || _appListener->keyboard()->isKeyDown(OIS::KC_ESCAPE) )
	{
		MyApplication::exit();
		return false;
	}

	float camSpeed = 10;
	if (_appListener->keyboard()->isKeyDown(OIS::KC_W))
	{
		_mainCamera->moveRelative(Ogre::Vector3(0,0,-1) * camSpeed * evt.timeSinceLastFrame);
	}
	if (_appListener->keyboard()->isKeyDown(OIS::KC_S))
	{
		_mainCamera->moveRelative(Ogre::Vector3(0,0,1) * camSpeed * evt.timeSinceLastFrame);
	}
	if (_appListener->keyboard()->isKeyDown(OIS::KC_A))
	{
		_mainCamera->moveRelative(Ogre::Vector3(-1,0,0) * camSpeed * evt.timeSinceLastFrame);
	}
	if (_appListener->keyboard()->isKeyDown(OIS::KC_D))
	{
		_mainCamera->moveRelative(Ogre::Vector3(1,0,0) * camSpeed * evt.timeSinceLastFrame);
	}
	
#pragma endregion cameraControls
	if (_appListener->mouse()->getMouseState().buttonDown( OIS::MouseButtonID::MB_Left ))
	{
		float rotX = _appListener->mouse()->getMouseState().X.rel * evt.timeSinceLastFrame * -1;
		float rotY = _appListener->mouse()->getMouseState().Y.rel * evt.timeSinceLastFrame * -1;

		_mainCamera->yaw(Ogre::Radian(rotX));
		_mainCamera->pitch(Ogre::Radian(rotY));

		float mousePosX = (float)_appListener->mouse()->getMouseState().X.abs /(float) _viewport->getActualWidth();
		float mousePosY = (float)_appListener->mouse()->getMouseState().Y.abs /(float) _viewport->getActualHeight();

		Ogre::Ray mouseRay = _mainCamera->getCameraToViewportRay(mousePosX, mousePosY);

		if (_movingStar != NULL)
		{
			_movingStar->followMouseRay(mouseRay);
			_movingStar->updateTwinPos();
		}
		
	}

	for (int i = 0; i < _nbrStars; ++i)
	{
		_boats[i]->update(evt);
	}


	return true;
}

