#include "Collider.h"
#include "GameObject.h"
#include "MathP.h"

//COLLIDER TRANSFORM
ColliderTransform::ColliderTransform(const Ogre::Vector3& position) : _position(position)
{

}

void ColliderTransform::setPosition(const Ogre::Vector3& pos)
{
	_position = Ogre::Vector3(pos);
}

const Ogre::Vector3& ColliderTransform::getAbsolutePosition()
{
	return _position;
}

const Ogre::Vector3& ColliderTransform::getAbsolutePosition() const
{
	return _position;
}

//COLLIDER TRANSFORM NODE
ColliderTransformNode::ColliderTransformNode(const Ogre::Vector3& position, const Ogre::Node* node): ColliderTransform(position), _node(node)
{

}

const Ogre::Vector3& ColliderTransformNode::getAbsolutePosition() const
{
	return _node->_getDerivedPosition() + _position;
}

const Ogre::Vector3& ColliderTransformNode::getAbsolutePosition()
{
	return _node->_getDerivedPosition() + _position;
}

const Ogre::Node* ColliderTransformNode::getNode()
{
	return _node;
}

const Ogre::Node* ColliderTransformNode::getNode() const
{
	return _node;
}


//BaseCOllider
BaseCollider::BaseCollider(ColliderTransform* transform, ColliderType type) :_transform(transform), _type(type)
{

}

BaseCollider::BaseCollider(const Ogre::Vector3& position, ColliderType type) : _transform(new ColliderTransform(position)), _type(type)
{

}

BaseCollider::BaseCollider(const Ogre::Vector3& position, const Ogre::Node* node, ColliderType type) : _transform(new ColliderTransformNode(position, node)), _type(type)
{

}

const BaseCollider::ColliderType& BaseCollider::getColliderType()
{
	return _type;
}

const BaseCollider::ColliderType& BaseCollider::getColliderType() const
{
	return _type;
}

ColliderTransform* BaseCollider::getTransform()
{
	return _transform;
}

const ColliderTransform* BaseCollider::getTransform() const
{
	return _transform;
}

//PointCollider
PointCollider::PointCollider(ColliderTransform* transform) : BaseCollider(transform, ColliderType::point)
{

}

PointCollider::PointCollider(const Ogre::Vector3& position) : BaseCollider(position, ColliderType::point)
{

}

PointCollider::PointCollider(const Ogre::Vector3& position, const Ogre::Node* node) : BaseCollider(position, node, ColliderType::point)
{

}

bool PointCollider::collideWith(const SphereCollider& sphere)
{
	return collision(sphere, _transform->getAbsolutePosition());
}

bool PointCollider::collideWith(const SphereCollider& sphere) const
{
	std::cout << "sphere" << std::endl;
	return collision(sphere, _transform->getAbsolutePosition());
}

bool PointCollider::collideWith(const BoxColliderAABB& box)
{
	return collision(box, _transform->getAbsolutePosition());
}

bool PointCollider::collideWith(const BoxColliderAABB& box) const
{
	return collision(box, _transform->getAbsolutePosition());
}

bool PointCollider::collideWith(const Ogre::Vector3& point)
{
	return false;
}

bool PointCollider::collideWith(const Ogre::Vector3& point) const
{
	return false;
}

bool PointCollider::collideWith(const PointCollider& Vector3)
{
	return false;
}

bool PointCollider::collideWith(const PointCollider& Vector3) const
{
	return false;
}

std::pair<bool, Ogre::Real> PointCollider::collideWith(const Ogre::Ray& ray)
{
	return std::make_pair(false, 0);
}

std::pair<bool, Ogre::Real> PointCollider::collideWith(const Ogre::Ray& ray) const
{
	return std::make_pair(false, 0);
}


bool PointCollider::collideWith(const BaseCollider* base)
{
	return	static_cast<const PointCollider &>( *this ).collideWith(base);
}

bool PointCollider::collideWith(const BaseCollider* base) const
{
	if (base->getColliderType() == ColliderType::sphere)
	{
		const SphereCollider* nCollider = static_cast<const SphereCollider*>(base); 
		return collision(*nCollider, _transform->getAbsolutePosition());
	}
	else if (base->getColliderType() == ColliderType::boxAABB)
	{
		const BoxColliderAABB* nCollider = static_cast<const BoxColliderAABB*>(base);
		return collision(*nCollider, _transform->getAbsolutePosition());
	}
	return FALSE;
}

//SphereCollider
SphereCollider::SphereCollider(ColliderTransform* transform, const float radius) : BaseCollider(transform, ColliderType::sphere), _radius(radius), _sphere(new Ogre::Sphere(transform->getAbsolutePosition(), radius))
{

}

SphereCollider::SphereCollider(const Ogre::Vector3& position, const float radius) : BaseCollider(position, ColliderType::sphere), _radius(radius), _sphere(new Ogre::Sphere(position, radius))
{

}

SphereCollider::SphereCollider(const Ogre::Vector3& position, const Ogre::Node* node, const float radius) : BaseCollider(position, node, ColliderType::sphere), _radius(radius), _sphere(new Ogre::Sphere(position, radius))
{

}

bool SphereCollider::collideWith(const SphereCollider& sphere)
{
	return collision(sphere, *this);
}

bool SphereCollider::collideWith(const SphereCollider& sphere) const
{
	return collision(sphere, *this);
}

bool SphereCollider::collideWith(const BoxColliderAABB& box)
{
	return collision(*this, box);
}

bool SphereCollider::collideWith(const BoxColliderAABB& box) const
{
	return collision(*this, box);
}

bool SphereCollider::collideWith(const Ogre::Vector3& point)
{
	return collision(*this, point);
}

bool SphereCollider::collideWith(const Ogre::Vector3& point) const
{
	return collision(*this, point);
}

bool SphereCollider::collideWith(const PointCollider& point)
{
	return collision(*this, point);
}

bool SphereCollider::collideWith(const PointCollider& point) const
{
	return collision(*this, point);
}

std::pair<bool, Ogre::Real> SphereCollider::collideWith(const Ogre::Ray& ray)
{
	_sphere->setCenter( _transform->getAbsolutePosition() );
	return MathP::rayIntersectSphere(ray, *_sphere);
}

std::pair<bool, Ogre::Real> SphereCollider::collideWith(const Ogre::Ray& ray) const
{
	_sphere->setCenter( _transform->getAbsolutePosition() );
	return MathP::rayIntersectSphere(ray, *_sphere);
}

bool SphereCollider::collideWith(const BaseCollider* base) const 
{
	if (base->getColliderType() == ColliderType::sphere)
	{
		const SphereCollider* nCollider = static_cast<const SphereCollider*>(base); 
		return collision(*nCollider, *this);
	}
	else if (base->getColliderType() == ColliderType::boxAABB)
	{
		const BoxColliderAABB* nCollider = static_cast<const BoxColliderAABB*>(base);
		return collision(*this, *nCollider);
	}
	else if (base->getColliderType() == ColliderType::point)
	{
		const PointCollider* nCollider = static_cast<const PointCollider*>(base);
		return collision(*this, nCollider->getTransform()->getAbsolutePosition());
	}
	return FALSE;
}

bool SphereCollider::collideWith(const BaseCollider* base) 
{
	return static_cast<const SphereCollider &>( *this ).collideWith(base);
}


const float& SphereCollider::getRadius()
{
	return _radius;
}

const float& SphereCollider::getRadius() const
{
	return _radius;
}

void SphereCollider::setRadius(const float nRadius)
{
	_radius = nRadius;
	_sphere->setRadius(nRadius);
}

//BoxAABBCollider
BoxColliderAABB::BoxColliderAABB(ColliderTransform* transform) : BaseCollider(transform, ColliderType::boxAABB)
{

}

BoxColliderAABB::BoxColliderAABB(const Ogre::Vector3& position) : BaseCollider(position, ColliderType::boxAABB)
{

}

BoxColliderAABB::BoxColliderAABB(const Ogre::Vector3& position, const Ogre::Node* node) : BaseCollider(position, node, ColliderType::boxAABB)
{

}

bool BoxColliderAABB::collideWith(const SphereCollider& sphere)
{
	return collision(sphere, *this);
}

bool BoxColliderAABB::collideWith(const SphereCollider& sphere) const
{
	return collision(sphere, *this);
}

bool BoxColliderAABB::collideWith(const BoxColliderAABB& box)
{
	return collision(*this, box);
}

bool BoxColliderAABB::collideWith(const BoxColliderAABB& box) const
{
	return collision(*this, box);
}

bool BoxColliderAABB::collideWith(const Ogre::Vector3& point)
{
	return collision(*this, point);
}

bool BoxColliderAABB::collideWith(const Ogre::Vector3& point) const
{
	return collision(*this, point);
}

bool BoxColliderAABB::collideWith(const PointCollider& point)
{
	return collision(*this, point);
}

bool BoxColliderAABB::collideWith(const PointCollider& point) const
{
	return collision(*this, point);
}

std::pair<bool, Ogre::Real> BoxColliderAABB::collideWith(const Ogre::Ray& ray) const
{
	Ogre::Vector3 boxPos = _transform->getAbsolutePosition();
	Ogre::Vector3 relativeScale = getScale() * 0.5;

	Ogre::AxisAlignedBox alignedBox(boxPos.x - relativeScale.x, boxPos.x - relativeScale.y, boxPos.z - relativeScale.z, boxPos.x + relativeScale.x, boxPos.y + relativeScale.y, boxPos.z + relativeScale.z);

	return Ogre::Math::intersects(ray, alignedBox);
}

std::pair<bool, Ogre::Real> BoxColliderAABB::collideWith(const Ogre::Ray& ray)
{
	return static_cast<const BoxColliderAABB &>( *this ).collideWith(ray);
}
bool BoxColliderAABB::collideWith(const BaseCollider* base)
{
	return static_cast<const BoxColliderAABB &>( *this ).collideWith(base);
}

bool BoxColliderAABB::collideWith(const BaseCollider* base) const
{
	if (base->getColliderType() == ColliderType::sphere)
	{
		const SphereCollider* nCollider = static_cast<const SphereCollider*>(base); 
		return collision(*nCollider, *this);
	}
	else if (base->getColliderType() == ColliderType::boxAABB)
	{
		const BoxColliderAABB* nCollider = static_cast<const BoxColliderAABB*>(base);
		return collision(*this, *nCollider);
	}
	else if (base->getColliderType() == ColliderType::point)
	{
		const PointCollider* nCollider = static_cast<const PointCollider*>(base);
		return collision(*this, *nCollider);
	}
	return FALSE;
}

const Ogre::Vector3& BoxColliderAABB::getScale()
{
	return _scale;
}

const Ogre::Vector3& BoxColliderAABB::getScale() const
{
	return _scale;
}

void BoxColliderAABB::setScale(const Ogre::Vector3 nScale)
{
	_scale = nScale;
}

//BOX-POINT

bool BaseCollider::collision(const BoxColliderAABB& box, const PointCollider& point)
{
	return collision(box, point.getTransform()->getAbsolutePosition());
}

bool BaseCollider::collision(const BoxColliderAABB& box, const Ogre::Vector3& point)
{
	Ogre::Vector3 boxPos = box.getTransform()->getAbsolutePosition();

	Ogre::Vector3 relativeScale = box.getScale() * 0.5;

	if ( point.x >= boxPos.x - relativeScale.x
	&& point.x < boxPos.x + relativeScale.x
	&& point.y >= boxPos.y - relativeScale.y
	&& point.y < boxPos.y + relativeScale.y
	&& point.z >= boxPos.z - relativeScale.z
	&& point.z < boxPos.z + relativeScale.z )
       return TRUE;
   else
       return FALSE;
}

//SPHERE - POINT

bool BaseCollider::collision(const SphereCollider& sphere, const PointCollider& point)
{
	return collision( sphere, point.getTransform()->getAbsolutePosition() );
}

bool BaseCollider::collision(const SphereCollider& sphere, const Ogre::Vector3& point)
{
	float d = sphere.getTransform()->getAbsolutePosition().distance(point);
	if ( d <= sphere.getRadius())
	{
		return TRUE;
	}
	return FALSE;
}

//SPHERE-SPHERE

bool BaseCollider::collision(const SphereCollider& sphereA, const SphereCollider& sphereB)
{
	float d = sphereA.getTransform()->getAbsolutePosition().distance(sphereB.getTransform()->getAbsolutePosition());
	if ( d <= sphereA.getRadius() + sphereB.getRadius() )
	{
		return TRUE;
	}
	return FALSE;
}

//Box-box
bool BaseCollider::collision(const BoxColliderAABB& boxA, const BoxColliderAABB& boxB)
{
	Ogre::Vector3 boxPosA = boxA.getTransform()->getAbsolutePosition();
	Ogre::Vector3 boxPosB = boxB.getTransform()->getAbsolutePosition();
	Ogre::Vector3 relativeScaleA = boxA.getScale() * 0.5;
	Ogre::Vector3 relativeScaleB = boxB.getScale() * 0.5;

	if ( boxPosA.x - relativeScaleA.x > boxPosB.x + relativeScaleB.x || boxPosA.x + relativeScaleA.x < boxPosB.x - relativeScaleB.x
		|| boxPosA.y - relativeScaleA.y > boxPosB.y + relativeScaleB.y || boxPosA.y + relativeScaleA.y < boxPosB.y - relativeScaleB.y
		|| boxPosA.z - relativeScaleA.z > boxPosB.z + relativeScaleB.z || boxPosA.z + relativeScaleA.z < boxPosB.z - relativeScaleB.z)
	{
		return FALSE;
	}
	return TRUE;
}

bool BaseCollider::collision(const SphereCollider& sphere, const BoxColliderAABB& box)
{
	return FALSE;
}