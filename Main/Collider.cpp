#include "Collider.h"
#include "GameObject.h"

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

const ColliderTransform* BaseCollider::getTransform()
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

bool PointCollider::collideWith(const BoxColliderAABB& box)
{
	return collision(box, _transform->getAbsolutePosition());
}

bool PointCollider::collideWith(const Ogre::Vector3& point)
{
	return false;
}

bool PointCollider::collideWith(const PointCollider& Vector3)
{
	return false;
}

bool PointCollider::collideWith(BaseCollider* base)
{
	if (base->getColliderType() == ColliderType::sphere)
	{
		SphereCollider* nCollider = dynamic_cast<SphereCollider*>(base); 
		return collision(*nCollider, _transform->getAbsolutePosition());
	}
	else if (base->getColliderType() == ColliderType::boxAABB)
	{
		BoxColliderAABB* nCollider = dynamic_cast<BoxColliderAABB*>(base);
		return collision(*nCollider, _transform->getAbsolutePosition());
	}
	return FALSE;
}

//SphereCollider
SphereCollider::SphereCollider(ColliderTransform* transform) : BaseCollider(transform, ColliderType::sphere)
{

}

SphereCollider::SphereCollider(const Ogre::Vector3& position) : BaseCollider(position, ColliderType::sphere)
{

}

SphereCollider::SphereCollider(const Ogre::Vector3& position, const Ogre::Node* node) : BaseCollider(position, node, ColliderType::sphere)
{

}

bool SphereCollider::collideWith(const SphereCollider& sphere)
{
	return collision(sphere, *this);
}

bool SphereCollider::collideWith(const BoxColliderAABB& box)
{
	return collision(*this, box);
}

bool SphereCollider::collideWith(const Ogre::Vector3& point)
{
	return collision(*this, point);
}

bool SphereCollider::collideWith(const PointCollider& point)
{
	return collision(*this, point);
}

bool SphereCollider::collideWith(BaseCollider* base)
{
	if (base->getColliderType() == ColliderType::sphere)
	{
		SphereCollider* nCollider = dynamic_cast<SphereCollider*>(base); 
		return collision(*nCollider, *this);
	}
	else if (base->getColliderType() == ColliderType::boxAABB)
	{
		BoxColliderAABB* nCollider = dynamic_cast<BoxColliderAABB*>(base);
		return collision(*this, *nCollider);
	}
	else if (base->getColliderType() == ColliderType::point)
	{
		PointCollider* nCollider = dynamic_cast<PointCollider*>(base);
		return collision(*this, nCollider->getTransform()->getAbsolutePosition());
	}
	return FALSE;
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

bool BoxColliderAABB::collideWith(const BoxColliderAABB& box)
{
	return collision(*this, box);
}

bool BoxColliderAABB::collideWith(const Ogre::Vector3& point)
{
	return collision(*this, point);
}

bool BoxColliderAABB::collideWith(const PointCollider& point)
{
	return collision(*this, point);
}

bool BoxColliderAABB::collideWith(BaseCollider* base)
{
	if (base->getColliderType() == ColliderType::sphere)
	{
		SphereCollider* nCollider = dynamic_cast<SphereCollider*>(base); 
		return collision(*nCollider, *this);
	}
	else if (base->getColliderType() == ColliderType::boxAABB)
	{
		BoxColliderAABB* nCollider = dynamic_cast<BoxColliderAABB*>(base);
		return collision(*this, *nCollider);
	}
	else if (base->getColliderType() == ColliderType::point)
	{
		PointCollider* nCollider = dynamic_cast<PointCollider*>(base);
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