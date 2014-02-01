#include "MainScene.h"
#include "MathP.h"
#include "MyApplication.h"
#include "GameObject.h"
#include "Collider.h"

MainScene::MainScene(Ogre::RenderWindow* win, Ogre::Root* root) : Scene(win, root), _stars(new GameObject*[_nbrStars])
{

}
MainScene::~MainScene(void)
{
	Scene::~Scene();
	delete [] _stars;
}

void MainScene::createCameras(Ogre::RenderWindow* win)
{
	_camera = _sceneManager->createCamera( "MainCamera" );
	_camera->setPosition( Ogre::Vector3( 0,20,0 ) );
	_camera->lookAt( 0, 15, -15 );
	_camera->setNearClipDistance(5);
			
	_viewport= win->addViewport(_camera);

	_camera->setAspectRatio( Ogre::Real( _viewport->getActualWidth() ) / Ogre::Real( _viewport->getActualHeight() ) );

	setMouseViewportSize(_viewport);
}



void MainScene::createScene()
{
	Ogre::ColourValue fadeColour(0.03f, 0.03f, 0.08f);
	_viewport->setBackgroundColour(fadeColour);
	_sceneManager->setFog(Ogre::FOG_LINEAR, fadeColour, 0.0, 200, 300);
	_sceneManager->setSkyDome(true, "Examples/NightSkyDome", 5, 4, 100);

	_skySphere = new Sphere(_camera->getDerivedPosition(), 30);

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

	createStars();
	
}

void MainScene::createStars()
{
	for (int i = 0; i < _nbrStars; ++i)
	{
		Ogre::Entity* ent = _sceneManager->createEntity("sinbad","Sinbad.mesh");
		Ogre::Ray ray(_camera->getDerivedPosition(), Vector3(0,0.2 ,-1));
		
		_stars[i] = new GameObject(this);
		_stars[i]->getNode()->attachObject(ent);

		std::pair<bool, Ogre::Real> t = MathP::rayIntersectSphere(ray, *_skySphere);
		Vector3 starPos;
		if (t.first)
			starPos = ray.getOrigin() + ray.getDirection() * MathP::rayIntersectSphere(ray, *_skySphere).second;
		else
			std::cerr<< "Camera is not inside SkySphere" << std::endl;

		_stars[i]->getNode()->setPosition(starPos);
		_stars[i]->getNode()->setScale(0.2,0.2,0.2);
	}



}


bool MainScene::frameStarted(const Ogre::FrameEvent& evt)
{
		MyFrameListener::frameStarted(evt); //parent call
#pragma region cameraControls

	if (_keyboard->isKeyDown(OIS::KC_F4) )
	{
		MyApplication::exit();
		return false;
	}
	
#pragma endregion cameraControls
	if (_mouse->getMouseState().buttonDown( OIS::MouseButtonID::MB_Left ))
	{
		
		float rotX = _mouse->getMouseState().X.rel * evt.timeSinceLastFrame * -1;
		float rotY = _mouse->getMouseState().Y.rel * evt.timeSinceLastFrame * -1;

		_camera->yaw(Ogre::Radian(rotX));
		_camera->pitch(Ogre::Radian(rotY));

		Vector3 direction = _camera->getDerivedOrientation() * Vector3::NEGATIVE_UNIT_Z;
		float mousePosX = (float)_mouse->getMouseState().X.abs /(float) _viewport->getActualWidth();
		float mousePosY = (float)_mouse->getMouseState().Y.abs /(float) _viewport->getActualHeight();

		Ogre::Ray mouseRay = _camera->getCameraToViewportRay(mousePosX, mousePosY);
		std::pair<bool, Real> intersectR = MathP::rayIntersectSphere(mouseRay, *_skySphere);

		Vector3 pos = mouseRay.getOrigin() + mouseRay.getDirection() * intersectR.second;
		
		if (_movingObject == NULL)
		{
			PointCollider posCollider(pos);
			for (int i = 0; i < _nbrStars; ++i)
			{
				if( posCollider.collideWith( _stars[i]->getCollider() ) )
				{
					_movingObject = _stars[i];
					std::cout<<"COLLIDE WITH STAR" << std::endl;
					break;
				}
			}
			
		}
		else
		{
			_movingObject->getNode()->_setDerivedPosition(pos);
		}
	}
	else if (_movingObject != NULL)
	{
		_movingObject = NULL;
	}
	

	return true;
}

