#pragma once
#include <vector>
#include "Physics/Collision/RigidBody.hpp"

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
		const std::vector<RigidBody*>& getBodies();
	private:
		std::vector<RigidBody*> m_bodies;
		bool m_gravity_enabled;
		Vector2 m_gravity;
	};
}