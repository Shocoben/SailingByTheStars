#pragma once
#include "Ogre.h"

class MathP
{
public :
	static std::pair<bool, Ogre::Real> rayIntersectSphere(const Ogre::Ray& ray, const Ogre::Sphere& s);
	static Ogre::Vector3 Vector3Lerp(Ogre::Vector3 &srcLocation, Ogre::Vector3 &destLocation, Ogre::Real Time);
	static Ogre::Vector3 Vector3Slerp(Ogre::Vector3 &srcLocation, Ogre::Vector3 &destLocation, Ogre::Real Time);

};




