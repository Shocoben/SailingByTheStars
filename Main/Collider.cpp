#include "Collider.h"


Collider::Collider(const GameObject* gameObject)
{
	_gameObject = gameObject;
}


Collider::~Collider(void)
{
}

const GameObject* Collider::getGameObject()
{
	return _gameObject;
}

const GameObject* Collider::getGameObject() const
{
	return _gameObject;
}

bool Collider::collision(const SphereCollider& sC, const Collider& c)
{
	float d = sC.getGameObject()->getNode()->_getDerivedPosition().distance(c.getGameObject()->getNode()->_getDerivedPosition());
	if ( d <= sC.getRadius() )
	{
		return TRUE;
	}
	return FALSE;
}

/*
bool Collider::collision(const SphereCollider& sC, const SphereCollider& sCBis)
{
	float d = sC.getGameObject()->getNode()->_getDerivedPosition().distance( sCBis.getGameObject()->getNode()->_getDerivedPosition() );
	if ( d <= sC.getRadius() + sCBis.getRadius() )
	{
		return TRUE;
	}
	return FALSE;
}

bool Collider::collisionAABB(const BoxCollider& bC, const BoxCollider& bCBis)
{
	Ogre::Vector3 bPos = bC.getGameObject()->getNode()->_getDerivedPosition();
	Ogre::Vector3 bBisPos = bCBis.getGameObject()->getNode()->_getDerivedPosition();
	Ogre::Vector3 relativeScale = bC.getScale() * 0.5;
	Ogre::Vector3 relativeScaleBis = bCBis.getScale() * 0.5;

	if ( bPos.x - relativeScale.x > bBisPos.x + relativeScaleBis.x || bPos.x + relativeScale.x < bBisPos.x - relativeScaleBis.x
		|| bPos.y - relativeScale.y > bBisPos.y + relativeScaleBis.y || bPos.y + relativeScale.y < bBisPos.y - relativeScaleBis.y
		|| bPos.z - relativeScale.z > bBisPos.z + relativeScaleBis.z || bPos.z + relativeScale.z < bBisPos.z - relativeScaleBis.z)
	return false;
}

bool Collider::collisionAABB(const BoxCollider& bC, const Collider& c)
{
	Ogre::Vector3 cPos = c.getGameObject()->getNode()->_getDerivedPosition();
	Ogre::Vector3 bCPos = bC.getGameObject()->getNode()->_getDerivedPosition();
	Ogre::Vector3 relativeScale = bC.getScale() * 0.5;

	if ( cPos.x >= bCPos.x - relativeScale.x
	&& cPos.x < bCPos.x + relativeScale.x
	&& cPos.y >= bCPos.y - relativeScale.y
	&& cPos.y < bCPos.y + relativeScale.y
	&& cPos.z >= bCPos.z - relativeScale.z
	&& cPos.z < bCPos.z + relativeScale.z )
       return TRUE;
   else
       return FALSE;
}
*/

const float& SphereCollider::getRadius()
{
	return _radius;
}

const float& SphereCollider::getRadius() const
{
	return _radius;
}

const Ogre::Vector3& BoxCollider::getScale()
{
	return _scale;
}

const Ogre::Vector3& BoxCollider::getScale() const
{
	return _scale;
}

void SphereCollider::setRadius(float nRadius)
{
	_radius = nRadius;
}