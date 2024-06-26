#pragma once
#include "Engine/LynxWindow.hpp"
#include "Engine/Scene.hpp"
#include "Collision/CollisionShapes.hpp"
#include "Collision/Detection/Collider.hpp"

namespace lynx
{
	class LynxEngine
	{
	public:
		static LynxEngine* getLynxEngine();
		LynxWindow* getWindow();
		void setCurrentScene(Scene* scene);
		Scene* getCurrentScene();
		void step(float dt);
		float getStepTime();
		void setIterationsPerStep(int ips);
		void enableDebugMode(bool debug_mode);
		void drawBodies();
		void attachDrawingProc(std::function<void(LynxWindow*)> procedure);
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
		std::function<void(LynxWindow* window)> m_drawing_proc;

		bool m_debug_mode;

		void broadCollisionPhase();
		void narrowCollisionPhase();
		void integrateAccel(float dt);
		void integrateVelocity(float dt);
		void separateBodies(const CollisionResult result);
		void resolveCollision(const CollisionResult result);
	};
}