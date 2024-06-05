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
		float getStepTime();
		void setIterationsPerStep(int ips);
		void enableDebugMode(bool debug_mode);
		void shutDown();
	private:
		LynxEngine();

		static LynxEngine* m_instance;
		Scene* m_cur_scene;
		LynxWindow m_window;
		sf::Clock m_clock;
		float m_step_time;
		int m_ips;
		float m_dt_coef;
		std::list<std::pair<RigidBody*, RigidBody*>> m_collision_pairs;

		bool m_debug_mode;

		void broadCollisionPhase();
		void narrowCollisionPhase();
		void integrateAccel(float dt);
		void integrateVelocity(float dt);
		void drawBodies();
		void separateBodies(const CollisionResult result);
		void resolveCollision(const CollisionResult result);
	};
}