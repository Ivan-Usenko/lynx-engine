#include "RigidBody.hpp"

namespace lynx
{
	void RigidBody::setCollisionShape(CollisionShape* shape)
	{
		m_collison_shape = shape;
	}

	CollisionShape* RigidBody::getCollisionShape()
	{
		return m_collison_shape;
	}

	sf::Vector2f RigidBody::getLinearVelocity()
	{
		return m_linear_velocity;
	}

	void RigidBody::setLinearVelocity(sf::Vector2f velocity)
	{
		m_linear_velocity = velocity;
	}

	float RigidBody::getAngularVelocity()
	{
		return m_angular_velocity;
	}

	void RigidBody::setAngularVelocity(float angular_velocity)
	{
		m_angular_velocity = angular_velocity;
	}

	float RigidBody::getInverseMass()
	{
		return m_inverse_mass;
	}

	void RigidBody::setInverseMass(float inverse_mass)
	{
		m_inverse_mass = inverse_mass;
	}

	float RigidBody::getRestitution()
	{
		return m_restitution;
	}

	void RigidBody::setRestitution(float restitution)
	{
		m_restitution = restitution;
	}
}