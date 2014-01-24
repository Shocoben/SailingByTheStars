#include "Scene.h"
#include "MathP.h"
#include "MyApplication.h"
#include "GameObject.h"
#include "Collider.h"

Scene::Scene(Ogre::RenderWindow* win, Ogre::Root* root) : MyFrameListener(win, root), _movementSpeed(50.0f)
{
	_sceneManager = root->createSceneManager(Ogre::ST_GENERIC);
	
	createCameras(win);
	createScene();
	
}

Scene::~Scene()
{

}

void Scene::createCameras(Ogre::RenderWindow* win)
{
	_camera = _sceneManager->createCamera( "MainCamera" );
	_camera->setPosition(Ogre::Vector3(0,20,0));
	_camera->lookAt(0,15,-30);
	_camera->setNearClipDistance(5);
			
	_viewport= win->addViewport(_camera);

	_camera->setAspectRatio( Ogre::Real( _viewport->getActualWidth() ) / Ogre::Real( _viewport->getActualHeight() ) );

	setMouseViewportSize(_viewport);
}

GameObject* nGO;
void Scene::createScene()
{
	Ogre::ColourValue fadeColour(0.03f, 0.03f, 0.08f);
	_viewport->setBackgroundColour(fadeColour);
	_sceneManager->setFog(Ogre::FOG_LINEAR, fadeColour, 0.0, 200, 300);
	_sceneManager->setSkyDome(true, "Examples/NightSkyDome", 5, 4, 100);

	_skySphere = new Sphere(Vector3(0,0,0), 50);

	_sceneManager->setAmbientLight(Ogre::ColourValue(0.2f,0.2f,0.2f));

	Ogre::Light* light = _sceneManager->createLight("sun");
	light->setType(Ogre::Light::LT_DIRECTIONAL);
	light->setDirection(1,-1, -1);

	_waterPlane = new Ogre::Plane(Ogre::Vector3::UNIT_Y, -10);

	Ogre::MeshManager::getSingleton().createPlane("WaterMesh", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,*_waterPlane, 500,500,150,150, true, 1, 7, 5, Ogre::Vector3::UNIT_Z);
	Ogre::Entity* waterEnt = _sceneManager->createEntity("WaterEnt", "WaterMesh");
	_sceneManager->getRootSceneNode()->createChildSceneNode()->attachObject(waterEnt);
	waterEnt->setMaterialName("Examples/Water2");

	
	Ogre::MeshManager::getSingleton().createPlane("SimplePlane", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,*_waterPlane, 1000,1000,1,1, true, 1, 1, 1, Ogre::Vector3::UNIT_Z);
	Ogre::Entity* groundEnt = _sceneManager->createEntity("WaterFloor", "SimplePlane");
	Ogre::SceneNode* _groundNode = _sceneManager->getRootSceneNode()->createChildSceneNode();
	_groundNode->translate(0,-15,0);
	_groundNode->attachObject(groundEnt);
	groundEnt->setMaterialName("OceanFloor");

	
	Ogre::Entity* ent = _sceneManager->createEntity("sinbad","Sinbad.mesh");
	
	nGO = new GameObject(this, "test");
	nGO->getNode()->attachObject(ent);
	nGO->getNode()->setPosition(0, 20, -6);
	nGO->getNode()->setScale(1,1,1);

	
}

Ogre::SceneManager* Scene::sceneManager()
{
	return _sceneManager;
}

bool Scene::frameStarted(const Ogre::FrameEvent& evt)
{
	MyFrameListener::frameStarted(evt);
	#pragma region cameraControls

		Ogre::Vector3 camTranslate(0,0,0);
		if (_keyboard->isKeyDown(OIS::KC_W))
		{
			camTranslate += Ogre::Vector3(0, 0, -1);
		}

		if (_keyboard->isKeyDown(OIS::KC_S))
		{
			camTranslate += Ogre::Vector3(0, 0, 1);
		}

		if (_keyboard->isKeyDown(OIS::KC_A))
		{
			camTranslate += Ogre::Vector3(-1, 0, 0);
		}

		if (_keyboard->isKeyDown(OIS::KC_D))
		{
			camTranslate += Ogre::Vector3(1, 0, 0);
		}

		if (_keyboard->isKeyDown(OIS::KC_F4) )
		{
			MyApplication::exit();
			return false;
		}

		_camera->moveRelative(camTranslate * evt.timeSinceLastFrame * _movementSpeed);


		float rotX = _mouse->getMouseState().X.rel * evt.timeSinceLastFrame * -1;
		float rotY = _mouse->getMouseState().Y.rel * evt.timeSinceLastFrame * -1;

		_camera->yaw(Ogre::Radian(rotX));
		//_camera->pitch(Ogre::Radian(rotY));
		
#pragma endregion cameraControls
	if (_mouse->getMouseState().buttonDown( OIS::MouseButtonID::MB_Left ))
	{
		float mousePosX = (float)_mouse->getMouseState().X.abs /(float) _viewport->getActualWidth();
		float mousePosY = (float)_mouse->getMouseState().Y.abs /(float) _viewport->getActualHeight();

		Ogre::Ray mouseRay = _camera->getCameraToViewportRay(mousePosX, mousePosY);
		
		
		std::pair<bool, Real> intersectR = MathP::rayIntersectSphere(mouseRay, *_skySphere);

		
	}
	

	return true;
}

int Scene::getLastGOListLength()
{
	return _gameObjectsList.size();
}

void Scene::addToScene(GameObject* gameObject)
{
	_gameObjectsList.push_back(gameObject);
}




void Scene::removeFromScene(GameObject* gameObject)
{
	std::vector<GameObject*>::iterator itGO = _gameObjectsList.begin() + gameObject->getSceneID();
	if (gameObject->getSceneID() < _gameObjectsList.size())
	{
		for(std::vector<GameObject*>::iterator it = itGO + 1; it != _gameObjectsList.end(); ++it) 
		{
			GameObject* cObj = *it;
			cObj->objectHaveBeenErasedFromlist(gameObject->getSceneID());
		}
	}
	_gameObjectsList.erase(itGO);


}