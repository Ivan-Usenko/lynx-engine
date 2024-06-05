#include "RigidBody.hpp"
#include "Math/LynxMath.hpp"

namespace lynx
{
	RigidBody::RigidBody()
	{
		m_collision_shape = nullptr;
		m_angular_velocity = 0.f;
		m_inverse_mass = 0.f;
		m_restitution = 1.f;
		m_inverse_inertia = 0.f;
		m_friction = 0.3f;
		m_sprite = nullptr;
	}

	void RigidBody::setCollisionShape(CollisionShape* shape)
	{
		m_collision_shape = shape;
		calcInverseInertia();
	}

	CollisionShape* RigidBody::getCollisionShape()
	{
		return m_collision_shape;
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
		calcInverseInertia();
	}

	float RigidBody::getRestitution()
	{
		return m_restitution;
	}

	void RigidBody::setRestitution(float restitution)
	{
		m_restitution = LynxMath::clamp(restitution, 0.f, 1.f);
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

	void RigidBody::applyAngularImpulse(Vector2 force, Vector2 lever)
	{
		m_angular_velocity += LynxMath::cross(lever, force) * m_inverse_inertia;
	}

	AABB RigidBody::calcAABB()
	{
		float min_x = std::numeric_limits<float>::max();
		float min_y = std::numeric_limits<float>::max();
		float max_x = std::numeric_limits<float>::min();
		float max_y = std::numeric_limits<float>::min();

		if (m_collision_shape)
		{
			CollisionShape::ShapeType type = m_collision_shape->getType();
			if (type == CollisionShape::Circle)
			{
				CollisionCircle* circle = (CollisionCircle*)m_collision_shape;
				min_x = getPosition().x - circle->getRadius();
				min_y = getPosition().y - circle->getRadius();
				max_x = getPosition().x + circle->getRadius();
				max_y = getPosition().y + circle->getRadius();
			}
			else if (type == CollisionShape::Box)
			{
				CollisionBox* box = (CollisionBox*)m_collision_shape;
				Vector2 vertices[4];
				box->calcBoxVertices(vertices, (lynx::Transform*)this);
				for (Vector2 v : vertices)
				{
					min_x = fminf(min_x, v.x);
					min_y = fminf(min_y, v.y);
					max_x = fmaxf(max_x, v.x);
					max_y = fmaxf(max_y, v.y);
				}
			}
		}

		return AABB(min_x, min_y, max_x, max_y);
	}

	float RigidBody::getInvInertia()
	{
		return m_inverse_inertia;
	}

	void RigidBody::calcInverseInertia()
	{
		if (m_collision_shape) m_inverse_inertia = m_collision_shape->getInvInertiaCoef() * m_inverse_mass;
		else m_inverse_inertia = 0.f;		
	}

	void RigidBody::setFriction(float friction)
	{
		m_friction = LynxMath::clamp(friction, 0.f, 1.f);
	}

	float RigidBody::getFriction()
	{
		return m_friction;
	}

	void RigidBody::setSprite(sf::Sprite* sprite)
	{
		m_sprite = sprite;
	}

	sf::Sprite* RigidBody::getSprite()
	{
		return m_sprite;
	}
}