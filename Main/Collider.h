#pragma once
#include "Ogre.h"

class BoxColliderAABB;
class SphereCollider;
class PointCollider;

class ColliderTransform
{
protected : 
	Ogre::Vector3 _position;
public : 
	ColliderTransform(const Ogre::Vector3& position);
	virtual const Ogre::Vector3& getAbsolutePosition();
	virtual const Ogre::Vector3& getAbsolutePosition() const;
	virtual void setPosition(const Ogre::Vector3& pos);
};


class ColliderTransformNode : public ColliderTransform
{
protected :
	const Ogre::Node* _node;
public:
	ColliderTransformNode(const Ogre::Vector3& position, const Ogre::Node* node);
	virtual const Ogre::Vector3& getAbsolutePosition();
	virtual const Ogre::Vector3& getAbsolutePosition() const;
	const Ogre::Node* getNode();
};



class BaseCollider
{
public : 
	enum ColliderType{none, point, sphere, boxAABB };

protected :
	ColliderTransform* _transform;
	ColliderType _type;
public :
	const ColliderType& getColliderType();
	const ColliderType& getColliderType() const;

	const ColliderTransform* getTransform();
	const ColliderTransform* getTransform() const;

	BaseCollider(ColliderTransform* transform, ColliderType type);
	BaseCollider(const Ogre::Vector3& position, ColliderType type);
	BaseCollider(const Ogre::Vector3& position, const Ogre::Node* node, ColliderType type);

	
	virtual bool collideWith(const BoxColliderAABB& box) = 0;
	virtual bool collideWith(const Ogre::Vector3& point) = 0;
	virtual bool collideWith(const SphereCollider& sphere) = 0;
	virtual bool collideWith(const PointCollider& point) = 0;

	static bool collision(const BoxColliderAABB& box, const Ogre::Vector3& point);
	static bool collision(const SphereCollider& sphere, const Ogre::Vector3& point);

	static bool collision(const BoxColliderAABB& box, const PointCollider& point);
	static bool collision(const SphereCollider& sphere, const PointCollider& point);



	static bool collision(const SphereCollider& sphere, const SphereCollider& sphereBis);
	static bool collision(const BoxColliderAABB& box, const BoxColliderAABB& boxBis);
	static bool collision(const SphereCollider& sphere, const BoxColliderAABB& box);

};


class PointCollider : public BaseCollider
{
public :
	PointCollider(ColliderTransform* transform);
	PointCollider(const Ogre::Vector3& position);
	PointCollider(const Ogre::Vector3& position, const Ogre::Node* node);
	

	virtual bool collideWith(const BoxColliderAABB& box);
	virtual bool collideWith(const Ogre::Vector3& point);
	virtual bool collideWith(const SphereCollider& sphere);
	virtual bool collideWith(const PointCollider& point);

	bool collideWith( BaseCollider* base);
};


class SphereCollider : public BaseCollider //There is a problem: I can't become a pointCollider
{
private :
	float _radius;
public : 
	SphereCollider(ColliderTransform* transform, const float radius);
	SphereCollider(const Ogre::Vector3& position, const float radius);
	SphereCollider(const Ogre::Vector3& position, const Ogre::Node* node, const float radius);

	virtual bool collideWith(const BoxColliderAABB& box);
	virtual bool collideWith(const Ogre::Vector3& point);
	virtual bool collideWith(const SphereCollider& sphere);
	virtual bool collideWith(const PointCollider& point);

	bool collideWith( BaseCollider* base);

	const float& getRadius();
	const float& getRadius() const;
	void setRadius(const float nRadius);
};


class BoxColliderAABB : public BaseCollider
{
private :
		Ogre::Vector3 _scale;
public :
	BoxColliderAABB(ColliderTransform* transform);
	BoxColliderAABB(const Ogre::Vector3& position);
	BoxColliderAABB(const Ogre::Vector3& position, const Ogre::Node* node);

	virtual bool collideWith(const BoxColliderAABB& box);
	virtual bool collideWith(const Ogre::Vector3& point);
	virtual bool collideWith(const SphereCollider& sphere);
	virtual bool collideWith(const PointCollider& point);

	bool collideWith( BaseCollider* base);

	const Ogre::Vector3& getScale();
	const Ogre::Vector3& getScale() const;
	void setScale(const Ogre::Vector3 nScale);
};

