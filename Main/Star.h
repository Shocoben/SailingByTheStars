#pragma once
#include "GameObject.h"

class StarRepresentation;

class Star : public GameObject
{
public :
	Star(Scene* myScene, Ogre::Sphere* skySphere, Ogre::Plane* waterPlane, Ogre::Plane* _skyPlane);
	StarRepresentation* collideWithMouse(Ogre::Ray* mouseRay);
	const StarRepresentation* getWaterStar();
	const StarRepresentation* getWaterStar() const;
protected :

	StarRepresentation* _waterStar;
	StarRepresentation* _skyStar;
	~Star(void);
};

class StarRepresentation : public GameObject
{
protected : 
	StarRepresentation* _twin;
	Ogre::Sphere* _skySphere;
	Ogre::Plane* _waterPlane;
	Ogre::Plane* _skyPlane;
public :
	StarRepresentation(Scene* myScene, StarRepresentation* twin, Ogre::Sphere* skySphere, Ogre::Plane* waterPlane, Ogre::Plane* _skyPlane) ;
	virtual void followMouseRay(const Ogre::Ray& ray) = 0;
	virtual void updateTwinPos() = 0;
	void setTwin(StarRepresentation* twin);
};

class WaterStar : public StarRepresentation
{
	public :
		WaterStar(Scene* myScene, StarRepresentation* twin, Ogre::Sphere* skySphere, Ogre::Plane* waterPlane, Ogre::Plane* _skyPlane);
		virtual void followMouseRay(const Ogre::Ray& ray);
		virtual void updateTwinPos();
};

class SkyStar : public StarRepresentation
{
	public :
		SkyStar(Scene* myScene, StarRepresentation* twin, Ogre::Sphere* skySphere, Ogre::Plane* waterPlane, Ogre::Plane* _skyPlane);
		virtual void followMouseRay(const Ogre::Ray& ray);
		virtual void updateTwinPos();
};