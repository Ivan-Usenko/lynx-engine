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
		sf::Clock m_clock;

		void integrateAccel(float dt);
		void integrateVelocity(float dt);
		void drawBodies();
		void drawInterface();
		void separateBodies(const CollisionResult result);
		void resolveCollision(const CollisionResult result);
	};
}