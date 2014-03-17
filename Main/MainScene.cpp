#include "MainScene.h"
#include "MathP.h"
#include "MyApplication.h"
#include "ApplicationListener.h"
#include "Star.h"
#include "Boat.h"
#include "Collider.h"
#include "OgreTerrain.h"
#include "TerrainGeneratorB.h"

MainScene::MainScene(MyApplication* app, const xml_node<>* rootNode) : Scene( app, rootNode), _nbrStars(std::atoi(_rootNode->first_node("Boats")->first_attribute("number")->value())),
_stars(new Star*[_nbrStars]), _movementSpeed(50.0f), _movingStar(NULL), _boats(new Boat*[_nbrStars])
{

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
	Scene::~Scene();
}

void MainScene::createCameras(Ogre::RenderWindow* win)
{
	_mainCamera = _sceneManager->createCamera( "MainCamera" );

	xml_node<>* CameraNode = _rootNode->first_node("Camera");
	_mainCamera->setPosition( Ogre::Vector3(atof(CameraNode->first_attribute("startX")->value()), atof(CameraNode->first_attribute("startY")->value()), atof(CameraNode->first_attribute("startZ")->value()) ) );
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
	createFogMesh();
}

void MainScene::readPixelFromTexture(Ogre::TexturePtr texturePtr,Ogre::Vector3* output)
{
	unsigned char* datas = new unsigned char[texturePtr->getWidth() * texturePtr->getHeight() * 3];
	texturePtr->getBuffer()->blitToMemory(PixelBox(texturePtr->getWidth(), texturePtr->getHeight(),1, PF_R8G8B8, datas));

	for (int y = 0; y < texturePtr->getHeight(); ++y)
	{
		for (int x = 0; x < texturePtr->getWidth(); ++x)
		{
			unsigned char r = datas[(x + y * texturePtr->getWidth()) * 3];
			unsigned char g = datas[(x + 1 + y * texturePtr->getWidth()) * 3];
			unsigned char b = datas[(x + 2 + y * texturePtr->getWidth()) * 3];

			std::cout << " r " << r << " g " << g << " b " << b << std::endl;
		}
	}
}

void MainScene::createFogMesh()
{
	xml_node<>* terrainNode = _rootNode->first_node("Planes");
	
	//Get the XML attributes for the segments of the plane and the minY
	int segX = atoi(terrainNode->first_attribute("fogPlaneSegX")->value()); 
	int segY = atoi(terrainNode->first_attribute("fogPlaneSegY")->value());
	double minY = atof(terrainNode->first_attribute("fogMinY")->value());
	double diffY = atof(terrainNode->first_attribute("fogDiffY")->value());

	//FogMesh
	Ogre::MeshPtr fogMesh = Ogre::MeshManager::getSingleton().createManual("FogPlane", "General");
	Ogre::SubMesh* subMesh = fogMesh->createSubMesh();

	//calculates the counts
	int nbrVertices = segX * segY;
	int nbrPositions = 3 * segX * segY;
	int nbrUVs = 2 * segX * segY;
	int nbrTriangles = (segX - 1) * (segY - 1) * 3 * 2;
	
	//init arrays
	float* positions= new float[nbrPositions];
	float* normals = new float[nbrPositions];
	float* uvs = new float[nbrUVs];
	unsigned short* triangles= new unsigned short[nbrTriangles];

	//Indexes
	int vertexI = 0;
	int cTriangle = 0;
	
	//needed pointers
	Vector3 *point = new Vector3(0,0,0); //actual point information
	Vector3* cNormal = new Vector3(0,0,0); //will be used for the normal

	float halfWorldSize = _terrain->getWorldSize() * 0.5; //will be used for the getPoint on the map
	for (int z = 0; z < segY; ++z)
	{
		for (int x = 0; x < segX; ++x)
		{
			int vArrayIndex = x * 3 + z * segX * 3;
			int uvArrayIndex = x * 2 + z * segX * 2;

			//Get the position on the terrain with a normalized relative pos ((0, 0) is bottom left and (1, 1) top right of the map)
			//Gives the UV too.
			float rPosX = ((float) x / (float) (segX - 1));
			float rPosZ = ((float) z / (float) (segY - 1));
			_terrain->getPoint(halfWorldSize * rPosX, halfWorldSize * rPosZ, point); //in getPoint function(0,0) is the bottom left of the map and (halfWorldSize, halfWordSize) is the top right of the map
			
			//position
			positions[vArrayIndex] = point->x;
			positions[vArrayIndex + 1] = (point->y > minY)? point->y + diffY: minY;
			positions[vArrayIndex + 2] = point->z;

			//normals
			normals[vArrayIndex] = 0;
			normals[vArrayIndex + 1] = 1;
			normals[vArrayIndex + 2] = 0;
			
			//UV
			uvs[uvArrayIndex] = rPosX;
			uvs[uvArrayIndex + 1] = rPosZ;

			//triangles
			if (x < segX - 1 && z < segY - 1)
			{
				
				triangles[cTriangle] = vertexI;
				triangles[cTriangle+1] = vertexI + segX + 1;
				triangles[cTriangle+2] = vertexI + segX;
				cTriangle += 3;

				triangles[cTriangle] = vertexI ;
				triangles[cTriangle+1] = vertexI + 1;
				triangles[cTriangle+2] = vertexI + segX + 1;
				cTriangle += 3;
			}

			vertexI++;
		}
	}
	
	fogMesh->sharedVertexData = new Ogre::VertexData;
	fogMesh->sharedVertexData->vertexCount = nbrVertices;

	Ogre::VertexDeclaration* decl = fogMesh->sharedVertexData->vertexDeclaration;
	size_t offset = 0;
	//positions
	decl->addElement(0, offset, Ogre::VET_FLOAT3, Ogre::VES_POSITION);
	offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT3);

	//Register verticesInfos  in buffers
	Ogre::HardwareVertexBufferSharedPtr posBuffer = Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(offset, fogMesh->sharedVertexData->vertexCount, Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY);
	posBuffer->writeData(0, posBuffer->getSizeInBytes(), positions);
	fogMesh->sharedVertexData->vertexBufferBinding->setBinding(0, posBuffer);

	//register normals
	offset = 0;
	decl->addElement(1, offset, Ogre::VET_FLOAT3, Ogre::VES_NORMAL);
	offset+= Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT3);

	Ogre::HardwareVertexBufferSharedPtr vNBuffer = Ogre::HardwareBufferManager::getSingletonPtr()->createVertexBuffer(offset, fogMesh->sharedVertexData->vertexCount, Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY);
	
	vNBuffer->writeData(0, vNBuffer->getSizeInBytes(), normals);
	fogMesh->sharedVertexData->vertexBufferBinding->setBinding(1, vNBuffer);
	
	//register UVs

	offset = 0;
	decl->addElement(2, offset, Ogre::VET_FLOAT2, Ogre::VES_TEXTURE_COORDINATES);
	offset+= Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT2);

	Ogre::HardwareVertexBufferSharedPtr UVBuffer = Ogre::HardwareBufferManager::getSingletonPtr()->createVertexBuffer(offset, fogMesh->sharedVertexData->vertexCount, Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY);
	UVBuffer->writeData(0, UVBuffer->getSizeInBytes(), uvs);

	fogMesh->sharedVertexData->vertexBufferBinding->setBinding(2, UVBuffer);

	//Register indexes
	Ogre::HardwareIndexBufferSharedPtr indexBuffer = Ogre::HardwareBufferManager::getSingleton().createIndexBuffer(HardwareIndexBuffer::IT_16BIT, nbrTriangles, Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY);
	indexBuffer->writeData(0, indexBuffer->getSizeInBytes(), triangles, true);

	subMesh->useSharedVertices = true;
	subMesh->indexData->indexStart = 0;
	subMesh->indexData->indexCount = nbrTriangles;
	subMesh->indexData->indexBuffer = indexBuffer;

	fogMesh->_setBounds(AxisAlignedBox(positions[0],minY, positions[nbrPositions-3 + 2], positions[nbrPositions-3] ,halfWorldSize, positions[2]));

	fogMesh->load();


	
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
	
	Ogre::MeshManager::getSingleton().createPlane("WaterMesh", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,*_waterPlane, atof(waterPlaneNode->first_attribute("waterMeshW")->value()), atof(waterPlaneNode->first_attribute("waterMeshH")->value()),150,150, true, 1, 7, 5, Ogre::Vector3::UNIT_Z);
	Ogre::Entity* waterEnt = _sceneManager->createEntity("WaterEnt", "WaterMesh");
	_sceneManager->getRootSceneNode()->createChildSceneNode()->attachObject(waterEnt);
	waterEnt->setMaterialName("Ocean2_Cg");
	
	//createFogMesh();
	
	createTerrain();
	createFog();
	createBoatsAndStars();
	
}

void MainScene::createFog()
{
	_fogMaterial = Ogre::MaterialManager::getSingletonPtr()->getByName("FogOfWarTwo");
	_fogTextureLink = _fogMaterial->getTechnique(0)->getPass(0)->getTextureUnitState(0);

	xml_node<>* planesNode = _rootNode->first_node("Planes");
	_fogTexture = Ogre::TextureManager::getSingletonPtr()->createManual("fogTexture", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, Ogre::TEX_TYPE_2D,
																		atoi(planesNode->first_attribute("fogPlaneSegX")->value()), atoi(planesNode->first_attribute("fogPlaneSegY")->value()), 0,
																		PF_X8R8G8B8, TU_DYNAMIC );

	_fogTexture->load();

	

	xml_node<>* textureNode = _rootNode->first_node("Textures");
	/// Lock the buffer so we can write to it
	_fogTexture->getBuffer()->lock(HardwareBuffer::HBL_NORMAL);
	
	const PixelBox &pb = _fogTexture->getBuffer()->getCurrentLock();

	/// Update the contents of pb here
	/// Image data starts at pb.data and has format pb.format
	/// Here we assume data.format is PF_X8R8G8B8 so we can address pixels as uint32.
	uint32 *data = static_cast<uint32*>(pb.data);
	size_t height = pb.getHeight();
	size_t width = pb.getWidth();
	size_t pitch = pb.rowPitch; // Skip between rows of image
	for(size_t y=0; y<height; ++y)
	{
		for(size_t x=0; x<width; ++x)
		{
			// 0xRRGGBB -> fill the buffer with yellow pixels
			data[pitch*y + x] = 0x00FF00;
		}
	}

	for(size_t x=0; x<50; ++x)
	{
		for(size_t y=0; y<2; ++y)
		{
			// 0xRRGGBB -> fill the buffer with yellow pixels
			data[pitch*y + x] = 0;
		}
	}

	/// Unlock the buffer again (frees it for use by the GPU)
	_fogTexture->getBuffer()->unlock();
	

	_fogTextureLink->setTextureName("fogTexture");	
	
	_fogEnt = _sceneManager->createEntity("Fog", "FogPlane");
	Ogre::SceneNode* _fogSceneNode = _sceneManager->getRootSceneNode()->createChildSceneNode("FogSceneNode");
	
	_fogEnt->setMaterial(_fogMaterial);
	_fogSceneNode->attachObject(_fogEnt);
	_fogParamPtr = _fogMaterial->getBestTechnique(0)->getPass(0)->getFragmentProgramParameters();

	

	/*
	if(_fogParamPtr->hasNamedParameters())
	{
		std::cout << "hello" << std::endl;
	}
	_fogParamPtr->setNamedConstant("clarity", clarity, 1, 1);*/
}

void MainScene::createBoatsAndStars()
{
	for (int i = 0; i < _nbrStars; ++i)
	{
		_stars[i] = new Star(this, _skySphere, _waterPhysicPlane, _skyPlane);

		Ogre::stringstream boatParamName;
		boatParamName << "boat" << i;
		_boats[i] = new Boat(this, _stars[i], _terrain, _fogParamPtr, boatParamName.str());
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

