#pragma once
#include <SFML/Graphics.hpp>
#include "Engine/Types.hpp"
#include "Physics/Collision/CollisionShapes.hpp"
#include "Physics/Collision/Detection/AABB.hpp"

namespace lynx
{
	class RigidBody : public Transform
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
		void applyAngularImpulse(Vector2 force, Vector2 lever);
		AABB calcAABB();
		float getInvInertia();
		void setFriction(float friction);
		float getFriction();
		void setSprite(sf::Sprite* sprite);
		sf::Sprite* getSprite();
	private:
		Vector2 m_linear_velocity;
		float m_angular_velocity;
		float m_inverse_mass;
		float m_inverse_inertia;
		float m_restitution;
		float m_friction;
		Vector2 m_force;
		CollisionShape* m_collision_shape;
		sf::Sprite* m_sprite;

		void calcInverseInertia();
	};
}