#include "RigidBody.hpp"

namespace lynx
{
	RigidBody::RigidBody()
	{
		m_collison_shape = nullptr;
		m_angular_velocity = 0.f;
		m_inverse_mass = 0.f;
		m_restitution = 1.f;
	}

	void RigidBody::setCollisionShape(CollisionShape* shape)
	{
		m_collison_shape = shape;
	}

	CollisionShape* RigidBody::getCollisionShape()
	{
		return m_collison_shape;
	}

	Vector2 RigidBody::getLinearVelocity()
	{
		return m_linear_velocity;
	}

	void RigidBody::setLinearVelocity(Vector2 velocity)
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

	void RigidBody::addForce(Vector2 force)
	{
		m_force += force;
	}

	Vector2 RigidBody::getForce()
	{
		return m_force;
	}

	void RigidBody::clearForce()
	{
		m_force = Vector2();
	}

	void RigidBody::applyImpulse(Vector2 force)
	{
		m_linear_velocity += force * m_inverse_mass;
	}
}