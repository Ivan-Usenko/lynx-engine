#pragma once
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include "Physics/Collision/CollisionShapes.hpp"

namespace lynx
{
	class RigidBody : public sf::Transformable
	{
	public:
		void setCollisionShape(CollisionShape* shape);
		CollisionShape* getCollisionShape();
		sf::Vector2f getLinearVelocity();
		void setLinearVelocity(sf::Vector2f velocity);
		float getAngularVelocity();
		void setAngularVelocity(float angular_velocity);
		float getInverseMass();
		void setInverseMass(float inverse_mass);
		float getRestitution();
		void setRestitution(float restitution);
	private:
		CollisionShape* m_collison_shape;
		sf::Vector2f m_linear_velocity;
		float m_angular_velocity;
		float m_inverse_mass;
		float m_restitution;
	};
}