#include "Ogre.h"
#pragma once


class MathP
{
public :
	static std::pair<bool, Ogre::Real> rayIntersectSphere(Ogre::Ray& ray, const Ogre::Sphere& s);
};


