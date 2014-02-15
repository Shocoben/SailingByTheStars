#include "Boat.h"
#include "Scene.h"
#include "Star.h"
#include "MathP.h"
#include "ApplicationListener.h"
#include "Collider.h"

int Boat::count = 0;
Boat::Boat(Scene* myScene, const Star* star, Ogre::Terrain* terrain) : GameObject(myScene), _repr( star->getWaterStar() ), _reactivity(1.5), _speed(3), _starPoint(new PointCollider(star->getWaterStar()->getNode()->_getDerivedPosition())), _starCollider(NULL), _terrain(terrain)
{
	_node->setPosition(0.2,star->getWaterStar()->getNode()->_getDerivedPosition().y,-20);
	_boatEnt = myScene->sceneManager()->createEntity("ColourCube");
	_boatEnt->setMaterialName("FogOfWar");
	_node->attachObject(_boatEnt);
	_node->setScale(0.4,0.4,0.4);

	_frontNode = _node->createChildSceneNode(-Ogre::Vector3::UNIT_Z * 1);
	Ogre::Entity* frontNodEnt = myScene->sceneManager()->createEntity("ColourCube");
	frontNodEnt->setMaterialName("Diffuse");
	_frontNode->attachObject(frontNodEnt);
	_frontNode->setScale(0.2,1,0.2);


	_lightNode = _node->createChildSceneNode(Ogre::Vector3::UNIT_Y * 3);

	std::stringstream lightName;
	lightName << "boatLight" << count;
	Ogre::Light* portLight = myScene->sceneManager()->createLight(lightName.str());
	portLight->setType(Ogre::Light::LT_POINT);
	portLight->setDiffuseColour(1,1,1);
	portLight->setSpecularColour(0.4,0.4,0.2);
	portLight->setAttenuation(800, 2.0, 0.5, 0.08);

	Ogre::BillboardSet* bSet = myScene->sceneManager()->createBillboardSet(1);
	bSet->setMaterialName("Examples/Flare");
	Billboard* b = bSet->createBillboard(0,0,0, Ogre::ColourValue(0.4,0.4,0.2, 0.2));

	_lightNode->attachObject(portLight);
	_lightNode->attachObject(bSet);
	_lightNode->scale(0.05,0.05,0.05);

	_starCollider = new SphereCollider(Vector3(0,0,0), _node, 0.6);

	count++;
}

void Boat::update(const Ogre::FrameEvent& evt)
{
	if (_myScene->getAppListener()->keyboard()->isKeyDown(OIS::KC_DOWN))
	{
		_reactivity-= 0.005f;
		std::cout << _reactivity << std::endl;
	}

	if (_myScene->getAppListener()->keyboard()->isKeyDown(OIS::KC_UP))
	{
		_reactivity += 0.005f;
		std::cout << _reactivity << std::endl;
	}

	_starPoint->getTransform()->setPosition( _repr->getNode()->_getDerivedPosition() );
	Ogre::Ray ray( _frontNode->_getDerivedPosition(), -Vector3::UNIT_Y );
	std::pair<bool, Ogre::Vector3> fIntersect = _terrain->rayIntersects( ray );
	bool intersectTerrain = false;

	if (!_starPoint->collideWith(_starCollider) && !intersectTerrain)
	{
		Vector3 forward = _node->_getDerivedOrientation() * Vector3::NEGATIVE_UNIT_Z;
		Vector3 targetForward = _repr->getNode()->_getDerivedPosition() - _node->_getDerivedPosition();
		targetForward.normalise();

		Vector3 nextForward = MathP::Vector3Slerp( forward, targetForward, evt.timeSinceLastFrame * _reactivity);
		nextForward.normalise();
		nextForward.y = 0;
		_node->setDirection(nextForward, Ogre::Node::TS_WORLD);
		if ( fIntersect.first )
		{
			if ( fIntersect.second.y < 8 )
			{
				_node->translate(Vector3::NEGATIVE_UNIT_Z * evt.timeSinceLastFrame * _speed , Ogre::Node::TS_LOCAL); 
			}
		}
		else
		{
			_node->translate(Vector3::NEGATIVE_UNIT_Z * evt.timeSinceLastFrame * _speed , Ogre::Node::TS_LOCAL); 
		}
		
	}
}
	

Boat::~Boat(void)
{
	delete _starPoint;
}
