#pragma once
#include "Engine/LynxWindow.hpp"
#include "Engine/Scene.hpp"
#include "Physics/Collision/CollisionShapes.hpp"
#include "Physics/Collision/Detection/Collider.hpp"

namespace lynx
{
	class LynxEngine
	{
	public:
		static LynxEngine* getLynxEngine();
		LynxWindow* getWindow();
		void setCurrentScene(Scene* scene);
		void step(float dt);
		void shutDown();
	private:
		LynxEngine();

		static LynxEngine* m_instance;
		Scene* m_cur_scene;
		LynxWindow m_window;

		void integrateAccel(float dt);
		void integrateVelocity(float dt);
		void drawBodies();
		bool isCollide(RigidBody* b1, RigidBody* b2, CollisionResult* result);
		void separateBodies(RigidBody* b1, RigidBody* b2, CollisionResult result);
		void resolveCollision(RigidBody* b1, RigidBody* b2, CollisionResult result);
	};
}