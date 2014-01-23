#pragma once
#include "Ogre.h"
class GameObject
{
public :
	const Ogre::Node* getNode();
	const Ogre::Node* getNode() const;
};

class SphereCollider;
class BoxCollider;

class Collider
{
private:
	const GameObject* _gameObject;
public:
	Collider(const GameObject* gameObject);
	const GameObject* getGameObject();
	const GameObject* getGameObject() const;

	static bool collision(const SphereCollider& sC, const Collider& c);
	static bool collision(const SphereCollider& sC, const SphereCollider& sCBis);
	static bool collisionAABB(const BoxCollider& bC, const SphereCollider& bCBis);
	static bool collisionAABB(const BoxCollider& bC, const Collider& c);
	static bool collisionAABB(const BoxCollider& bC, const BoxCollider& c);

	~Collider(void);
};

class BoxCollider : public Collider
{
	private :
		Ogre::Vector3 _scale;
	public :
		const Ogre::Vector3& getScale();
		const Ogre::Vector3& getScale() const;
};

class SphereCollider : public Collider
{
private :
	float _radius;
public :
	const float& getRadius();
	const float& getRadius() const;
	void setRadius(float nRadius);
};
