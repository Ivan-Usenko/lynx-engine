#include "LynxEngine.hpp"
#include "Math/LynxMath.hpp"
#include <algorithm>

namespace lynx
{
	LynxEngine* LynxEngine::m_instance = nullptr;

	LynxEngine::LynxEngine()
	{
		m_window.create(sf::VideoMode(800, 600), "Lynx Engine");
		m_cur_scene = nullptr;
		m_window.setFramerateLimit(240);
		m_step_time = 1.f / 240.f;
		setIterationsPerStep(10);
		m_debug_mode = false;

		attachDrawingProc([&](LynxWindow* window) { drawBodies(); });
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

	void LynxEngine::setIterationsPerStep(int ips)
	{
		m_ips = ips;
		m_dt_coef = 1.f / (float)m_ips;
	}

	LynxWindow* LynxEngine::getWindow()
	{
		return &m_window;
	}

	void LynxEngine::step(float dt)
	{
		// Handling events
		m_window.handleEvents();

		dt *= m_dt_coef;
		for (int i = 0; i < m_ips; i++)
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
		if (m_drawing_proc) m_drawing_proc(&m_window);
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

				if (Collider::isAABBsOverlap(&a1, &a2))
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
					auto p = std::pair<Vector2, Vector2>(result.contact[i], result.normal);
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
			body->rotate(LynxMath::toDegrees(body->getAngularVelocity() * dt));
		}
	}

	void LynxEngine::drawBodies()
	{
		if (!m_cur_scene) return;
		for (RigidBody* body : m_cur_scene->getBodies())
		{
			sf::Sprite* sprite = body->getSprite();
			if (sprite)
			{
				sprite->setPosition(body->getPosition());
				sprite->setRotation(body->getRotation());
				sprite->setOrigin(sprite->getTextureRect().width / 2.f, sprite->getTextureRect().height / 2.f);
				m_window.draw(*sprite);
			}

			if (m_debug_mode)
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
	}

	void LynxEngine::attachDrawingProc(std::function<void(LynxWindow*)> procedure)
	{
		m_drawing_proc = procedure;
	}

	void LynxEngine::enableDebugMode(bool debug_mode)
	{
		m_debug_mode = debug_mode;
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
		if (inv_mass_sum)
		{
			Vector2 impulses[2];
			float imp_magnitudes[2];
			Vector2 r1_arr[2];
			Vector2 r2_arr[2];
			float e = fminf(b1->getRestitution(), b2->getRestitution());
			float inv_inert1 = b1->getInvInertia();
			float inv_inert2 = b2->getInvInertia();
			float frict_coef = (b1->getFriction() + b2->getFriction()) / 2.f;

			// Calculate normal impulse
			for (int i = 0; i < result.contact_count; i++)
			{
				Vector2 r1 = result.contact[i] - b1->getPosition();
				Vector2 r2 = result.contact[i] - b2->getPosition();

				Vector2 r1_p = Vector2(-r1.y, r1.x);
				Vector2 r2_p = Vector2(-r2.y, r2.x);

				Vector2 ang_lv1 = b1->getAngularVelocity() * r1_p;
				Vector2 ang_lv2 = b2->getAngularVelocity() * r2_p;

				Vector2 rel_v = (b2->getLinearVelocity() + ang_lv2) - (b1->getLinearVelocity() + ang_lv1);
				float cv = LynxMath::dot(rel_v, result.normal);

				if (cv > 0.f) return;

				float inert1 = powf(LynxMath::dot(r1_p, result.normal), 2.f) * inv_inert1;
				float inert2 = powf(LynxMath::dot(r2_p, result.normal), 2.f) * inv_inert2;

				float imp_magnitude = ((-(1.f + e) * cv) / (inv_mass_sum + inert1 + inert2)) / (float)result.contact_count;
				Vector2 impulse = imp_magnitude * result.normal;
				impulses[i] = impulse;
				imp_magnitudes[i] = imp_magnitude;
				r1_arr[i] = r1;
				r2_arr[i] = r2;
			}

			// Apply normal impulse
			for (int i = 0; i < result.contact_count; i++)
			{
				// Improve stacked boxes behavior
				if (LynxMath::equalf(impulses[i].x, 0.f)) impulses[i].x = 0.f;
				if (LynxMath::equalf(impulses[i].y, 0.f)) impulses[i].y = 0.f;

				b1->applyImpulse(-impulses[i]);
				b2->applyImpulse(impulses[i]);
				b1->applyAngularImpulse(-impulses[i], r1_arr[i]);
				b2->applyAngularImpulse(impulses[i], r2_arr[i]);
			}

			// Calculate tangent impulse
			impulses[0] = impulses[1] = Vector2();
			for (int i = 0; i < result.contact_count; i++)
			{
				Vector2 r1 = r1_arr[i];
				Vector2 r2 = r2_arr[i];

				Vector2 r1_p = Vector2(-r1.y, r1.x);
				Vector2 r2_p = Vector2(-r2.y, r2.x);

				Vector2 ang_lv1 = b1->getAngularVelocity() * r1_p;
				Vector2 ang_lv2 = b2->getAngularVelocity() * r2_p;

				Vector2 rel_v = (b2->getLinearVelocity() + ang_lv2) - (b1->getLinearVelocity() + ang_lv1);
				
				Vector2 tangent = rel_v - LynxMath::dot(rel_v, result.normal) * result.normal;

				if (LynxMath::equalf(tangent, Vector2())) continue;
				else tangent = LynxMath::normilize(tangent);

				float inert1 = powf(LynxMath::dot(r1_p, tangent), 2.f) * inv_inert1;
				float inert2 = powf(LynxMath::dot(r2_p, tangent), 2.f) * inv_inert2;

				float f_imp_magnitude = (-LynxMath::dot(rel_v, tangent) / (inv_mass_sum + inert1 + inert2)) / (float)result.contact_count;
				Vector2 f_impulse;

				if (fabsf(f_imp_magnitude) <= imp_magnitudes[i] * frict_coef) f_impulse = f_imp_magnitude * tangent * frict_coef;
				else f_impulse = -imp_magnitudes[i] * tangent * frict_coef * 0.8f;
				
				impulses[i] = f_impulse;
			}

			// Apply tangent impulse
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