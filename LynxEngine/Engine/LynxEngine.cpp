#include "LynxEngine.hpp"
#include "Math/LynxMath.hpp"
#include <iomanip>
#include <sstream>
#include <algorithm>

namespace lynx
{
	LynxEngine* LynxEngine::m_instance = nullptr;

	LynxEngine::LynxEngine()
	{
		m_cur_scene = nullptr;
		m_window.setFramerateLimit(240);
		m_step_time = 1.f / 240.f;
	}

	LynxEngine* LynxEngine::getLynxEngine()
	{
		if (!m_instance) m_instance = new LynxEngine();
		return m_instance;
	}

	void LynxEngine::setCurrentScene(Scene* scene)
	{
		m_cur_scene = scene;
	}

	void LynxEngine::shutDown()
	{
		if (m_instance) delete m_instance;
	}

	LynxWindow* LynxEngine::getWindow()
	{
		return &m_window;
	}

	void LynxEngine::step(float dt)
	{
		// Handling events
		m_window.handleEvents();

		dt *= 0.1f;
		for (int i = 0; i < 10; i++)
		{
			// Movement
			integrateAccel(dt);
			integrateVelocity(dt);

			// Collision
			broadCollisionPhase();
			narrowCollisionPhase();
		}
		
		// Drawing
		m_window.clear();
		drawBodies();
		for (Vector2& c : m_contacts) m_window.drawRectangle(c.x, c.y, 3.f, 3.f, 0.f, sf::Color::White, sf::Color::Red);
		m_contacts.clear();
		drawInterface();
		m_window.display();

		m_step_time = m_clock.restart().asSeconds();
	}

	float LynxEngine::getStepTime()
	{
		return m_step_time;
	}


	void LynxEngine::broadCollisionPhase()
	{
		std::list<RigidBody*> bodies = m_cur_scene->getBodies();
		for (auto i = bodies.begin(); i != bodies.end(); i++)
		{
			RigidBody* b1 = *i;
			AABB a1 = b1->calcAABB();
			for (auto j = i; j != bodies.end(); j++)
			{
				if (j == i) continue;
				RigidBody* b2 = *j;
				AABB a2 = b2->calcAABB();

				if (Collider::isAABBsOverlap(a1, a2))
				{
					m_collision_pairs.push_back(std::pair(b1, b2));
				}
			}
		}
	}

	void LynxEngine::narrowCollisionPhase()
	{
		for (auto& pair : m_collision_pairs)
		{
			RigidBody* b1 = pair.first;
			RigidBody* b2 = pair.second;
			CollisionResult result;
			if (Collider::isBodiesCollide(b1, b2, &result))
			{
				separateBodies(result);
				Collider::findContactPoints(&result);
				for (int i = 0; i < result.contact_count; i++)
				{
					if (std::find(m_contacts.begin(), m_contacts.end(), result.contact[i]) == m_contacts.end())
						m_contacts.push_back(result.contact[i]);
				}
				resolveCollision(result);
			}
		}
		m_collision_pairs.clear();
	}

	void LynxEngine::integrateAccel(float dt)
	{
		if (!m_cur_scene) return;
		for (RigidBody* body : m_cur_scene->getBodies())
		{
			if (body->getInverseMass() > 0.f)
			{
				Vector2 vel = body->getForce() * body->getInverseMass();
				if (m_cur_scene->isGravityEnabled()) vel += m_cur_scene->getGravity();
				body->setLinearVelocity(body->getLinearVelocity() + vel * dt);
			}
		}
	}

	void LynxEngine::integrateVelocity(float dt)
	{
		if (!m_cur_scene) return;
		for (RigidBody* body : m_cur_scene->getBodies())
		{
			body->move(body->getLinearVelocity() * dt);
			body->rotate(body->getAngularVelocity() * dt);
		}
	}

	void LynxEngine::drawBodies()
	{
		if (!m_cur_scene) return;
		for (RigidBody* body : m_cur_scene->getBodies())
		{
			CollisionShape* shape = body->getCollisionShape();
			if (!shape) continue;
			CollisionShape::ShapeType type = shape->getType();
			if (type == CollisionShape::Box)
			{
				CollisionBox* box = (CollisionBox*)shape;
				m_window.drawRectangle(body->getPosition().x, body->getPosition().y, box->getSize().x, box->getSize().y, body->getRotation());
			}
			else if (type == CollisionShape::Circle)
			{
				CollisionCircle* circle = (CollisionCircle*)shape;
				m_window.drawCircle(body->getPosition().x, body->getPosition().y, circle->getRadius(), body->getRotation());
			}
		}
	}

	void LynxEngine::drawInterface()
	{
		std::stringstream ss;
		ss << "DEBUG\n";
		ss << "Time step: " << std::fixed << std::setprecision(4) << getStepTime() << std::endl;
		ss << "Bodies count: " << m_cur_scene->getBodies().size();
		m_window.drawLabel(ss.str(), 15u);
	}

	void LynxEngine::separateBodies(const CollisionResult result)
	{
		RigidBody* b1 = result.body1;
		RigidBody* b2 = result.body2;
		float inv_mass_sum = b1->getInverseMass() + b2->getInverseMass();
		if (inv_mass_sum > 0.f)
		{
			b1->move(-result.normal * result.depth * (b1->getInverseMass() / inv_mass_sum));
			b2->move(result.normal * result.depth * (b2->getInverseMass() / inv_mass_sum));
		}
	}

	void LynxEngine::resolveCollision(const CollisionResult result)
	{
		RigidBody* b1 = result.body1;
		RigidBody* b2 = result.body2;

		float inv_mass_sum = b1->getInverseMass() + b2->getInverseMass();
		Vector2 impulses[2];
		Vector2 r1_arr[2];
		Vector2 r2_arr[2];
		float e = fminf(b1->getRestitution(), b2->getRestitution());
		float inv_inert1 = b1->getInvInertia();
		float inv_inert2 = b2->getInvInertia();
		if (inv_mass_sum)
		{
			for (int i = 0; i < result.contact_count; i++)
			{
				Vector2 r1 = result.contact[i] - b1->getPosition();
				Vector2 r2 = result.contact[i] - b2->getPosition();

				Vector2 r1_p = Vector2(-r1.y, r1.x);
				Vector2 r2_p = Vector2(-r2.y, r2.x);

				Vector2 ang_lv1 = b1->getAngularVelocity() * r1_p;
				Vector2 ang_lv2 = b2->getAngularVelocity() * r2_p;

				Vector2 rel_v = (b2->getLinearVelocity() + ang_lv2) - (b1->getLinearVelocity() + ang_lv2);
				float cv = LynxMath::dot(rel_v, result.normal);

				if (cv > 0.f) return;

				float inert1 = powf(LynxMath::dot(r1_p, result.normal), 2.f) * inv_inert1;
				float inert2 = powf(LynxMath::dot(r2_p, result.normal), 2.f) * inv_inert2;

				Vector2 impulse = ((-(1 + e) * cv) / (inv_mass_sum + inert1 + inert2)) * result.normal;
				impulses[i] = impulse / (float)result.contact_count;
				r1_arr[i] = r1;
				r2_arr[i] = r2;
			}

			for (int i = 0; i < result.contact_count; i++)
			{
				b1->applyImpulse(-impulses[i]);
				b2->applyImpulse(impulses[i]);
				b1->applyAngularImpulse(-impulses[i], r1_arr[i]);
				b2->applyAngularImpulse(impulses[i], r2_arr[i]);
			}
		}
	}
}