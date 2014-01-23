#pragma once
#include "Ogre.h"

class MathP
{
public :
	static std::pair<bool, Ogre::Real> rayIntersectSphere(Ogre::Ray& ray, const Ogre::Sphere& s);
};


