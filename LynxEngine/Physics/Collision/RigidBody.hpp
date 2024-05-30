#pragma once
#include "Engine/Types.hpp"
#include "Physics/Collision/CollisionShapes.hpp"

namespace lynx
{
	class RigidBody : public sf::Transformable
	{
	public:
		RigidBody();
		void setCollisionShape(CollisionShape* shape);
		CollisionShape* getCollisionShape();
		Vector2 getLinearVelocity();
		void setLinearVelocity(Vector2 velocity);
		float getAngularVelocity();
		void setAngularVelocity(float angular_velocity);
		float getInverseMass();
		void setInverseMass(float inverse_mass);
		float getRestitution();
		void setRestitution(float restitution);
		void addForce(Vector2 force);
		Vector2 getForce();
		void clearForce();
		void applyImpulse(Vector2 force);
	private:
		CollisionShape* m_collison_shape;
		Vector2 m_linear_velocity;
		float m_angular_velocity;
		float m_inverse_mass;
		float m_restitution;
		Vector2 m_force;
	};
}