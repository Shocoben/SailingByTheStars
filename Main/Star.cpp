#include "Star.h"
#include "Collider.h"
#include "MathP.h"
#include "Scene.h"


Star::Star(Scene* myScene, Ogre::Sphere* skySphere, Ogre::Plane* waterPlane, Ogre::Plane* skyPlane) : GameObject(myScene), _waterStar( new WaterStar(myScene, NULL, skySphere, waterPlane, skyPlane) ), _skyStar(new SkyStar(myScene, _waterStar, skySphere, waterPlane, skyPlane))
{
	Ogre::Ray ray(myScene->getMainCamera()->getDerivedPosition(), Vector3(0,-0.8 ,-1));
	_waterStar->getNode()->_setDerivedPosition(ray.getPoint(Math::intersects(ray, *waterPlane).second));
	_waterStar->updateTwinPos();
}

const StarRepresentation* Star::getWaterStar()
{
	return _waterStar;
}

const StarRepresentation* Star::getWaterStar() const
{
	return _waterStar;
}

StarRepresentation* Star::collideWithMouse(Ogre::Ray* mouseRay)
{
	if (_skyStar->getCollider()->collideWith(*mouseRay).first)
	{
		return _skyStar;
	}
	else if (_waterStar->getCollider()->collideWith(*mouseRay).first)
	{
		return _waterStar;
	}
	
	return NULL;
}

void StarRepresentation::setTwin(StarRepresentation* twin)
{
	_twin = twin;
}

StarRepresentation::StarRepresentation(Scene* myScene, StarRepresentation* twin, Ogre::Sphere* skySphere, Ogre::Plane* waterPlane, Ogre::Plane* skyPlane) : GameObject(myScene), _twin(twin), _skySphere(skySphere), _waterPlane(waterPlane), _skyPlane(skyPlane)
{
	//Ogre::Entity* ent = _myScene->sceneManager()->createEntity("Sinbad.mesh");

	Ogre::BillboardSet* bSet = _myScene->sceneManager()->createBillboardSet(1);
	bSet->setMaterialName("SailingByTheStars/Star");
	Billboard* b = bSet->createBillboard(0,0,0, Ogre::ColourValue(0.4,0.8,0.9, 0.7));
	
	bSet->setBounds(Ogre::AxisAlignedBox(Ogre::AxisAlignedBox::EXTENT_INFINITE), 1000);


	_node->attachObject(bSet);
	_node->setScale(0.08,0.08,0.08);
	if (twin != NULL)
	{
		twin->setTwin(this);
	}
}

WaterStar::WaterStar(Scene* myScene, StarRepresentation* twin, Ogre::Sphere* skySphere, Ogre::Plane* waterPlane,  Ogre::Plane* skyPlane) : StarRepresentation(myScene, twin, skySphere, waterPlane, skyPlane)
{

}

void WaterStar::followMouseRay(const Ogre::Ray& mouseRay)
{
	std::pair<bool, Real> intersectR = Math::intersects(mouseRay, *_waterPlane);
	if (intersectR.first)
	{
		Vector3 pos = mouseRay.getPoint(intersectR.second);
		/*if (pos.x < -50)
			pos.x = -50;
		if (pos.x > 50)
			pos.x = 50;
		if (pos.z > 50)
			pos.z = 50;
		if (pos.z < -50)
			pos.z = - 50;*/
		
		std::cout<< " x " << pos.x << " y " << pos.y << " z " << pos.z << std::endl;
		_node->setPosition(pos);
	}
}

void WaterStar::updateTwinPos()
{
	Ogre::Ray rayToSphere(_node->_getDerivedPosition(), Ogre::Vector3::UNIT_Y);
	//std::pair<bool, Real> intersectR = MathP::rayIntersectSphere(rayToSphere, *_skySphere);
	std::pair<bool, Real> intersectR = Math::intersects(rayToSphere, *_skyPlane);
	if (intersectR.first)
		_twin->getNode()->setPosition( rayToSphere.getPoint(intersectR.second) );
	else
		std::cerr << "Don't collide with sphere" << std::endl;
}

SkyStar::SkyStar(Scene* myScene, StarRepresentation* twin, Ogre::Sphere* skySphere, Ogre::Plane* waterPlane,  Ogre::Plane* skyPlane) : StarRepresentation(myScene, twin, skySphere, waterPlane, skyPlane)
{

}

void SkyStar::updateTwinPos()
{
	Ogre::Ray rayToPlane(_node->_getDerivedPosition(), -Ogre::Vector3::UNIT_Y);
	std::pair<bool, Ogre::Real> intersectR = Math::intersects(rayToPlane, *_waterPlane);
	Vector3 pos = rayToPlane.getPoint( intersectR.second ) ;
	
	_twin->getNode()->setPosition( pos );
}

void SkyStar::followMouseRay(const Ogre::Ray& mouseRay)
{
	std::pair<bool, Real> intersectPlane = Math::intersects(mouseRay, *_waterPlane);
	if (!intersectPlane.first)
	{
		std::pair<bool, Real> intersectR = Math::intersects(mouseRay, *_skyPlane);
		Vector3 pos = mouseRay.getOrigin() + mouseRay.getDirection() * intersectR.second;
		if (pos.x < -50)
			pos.x = -50;
		if (pos.x > 50)
			pos.x = 50;
		if (pos.z > 50)
			pos.z = 50;
		if (pos.z < -50)
			pos.z = - 50;
		_node->_setDerivedPosition(pos);
	}
	
}

Star::~Star(void)
{
	delete _waterStar;
	delete _skyStar;
}
