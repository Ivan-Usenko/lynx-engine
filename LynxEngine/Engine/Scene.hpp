#pragma once
#include <list>
#include "Physics/RigidBody.hpp"

namespace lynx
{
	class Scene
	{
	public:
		Scene();
		void setGravity(Vector2 gravity);
		Vector2 getGravity();
		void enableGravity(bool enable);
		bool isGravityEnabled();
		void addBody(RigidBody* body);
		void removeBody(RigidBody* body);
		void clearBoies();
		const std::list<RigidBody*>& getBodies();
	private:
		std::list<RigidBody*> m_bodies;
		bool m_gravity_enabled;
		Vector2 m_gravity;
	};
}