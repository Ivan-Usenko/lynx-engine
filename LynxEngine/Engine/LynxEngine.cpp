#include "LynxEngine.hpp"
#include "Math/LynxMath.hpp"

namespace lynx
{
	LynxEngine* LynxEngine::m_instance = nullptr;

	LynxEngine::LynxEngine()
	{
		m_cur_scene = nullptr;
		m_window.setFramerateLimit(240.f);
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
		m_window.handleEvents();

		integrateAccel(dt);
		integrateVelocity(dt);

		std::vector<RigidBody*> bodies = m_cur_scene->getBodies();
		for (int i = 0; i < bodies.size() - 1; i++)
		{
			for (int j = i + 1; j < bodies.size(); j++)
			{
				CollisionResult result;
				if (isCollide(bodies[i], bodies[j], &result))
				{
					separateBodies(bodies[i], bodies[j], result);
					resolveCollision(bodies[i], bodies[j], result);
				}
			}
		}

		m_window.clear();
		drawBodies();
		m_window.display();
	}

	void LynxEngine::integrateAccel(float dt)
	{
		if (!m_cur_scene) return;
		for (RigidBody* body : m_cur_scene->getBodies())
		{
			Vector2 vel = body->getForce() * body->getInverseMass() * dt;
			if (m_cur_scene->isGravityEnabled() && body->getInverseMass()) vel += m_cur_scene->getGravity();
			body->setLinearVelocity(body->getLinearVelocity() + vel * dt);
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
				m_window.drawCircle(body->getPosition().x, body->getPosition().y, circle->getRadius());
			}
		}
	}

	bool LynxEngine::isCollide(RigidBody* b1, RigidBody* b2, CollisionResult* result)
	{
		CollisionShape* s1 = b1->getCollisionShape();
		CollisionShape* s2 = b2->getCollisionShape();

		if (s1 && s2)
		{
			CollisionShape::ShapeType st1 = s1->getType();
			CollisionShape::ShapeType st2 = s2->getType();
			Transform* t1 = b1;
			Transform* t2 = b2;

			if (st1 == CollisionShape::Circle)
			{
				if (st2 == CollisionShape::Circle)
				{
					return Collider::intersectCircles(*t1, *(CollisionCircle*)s1, *t2, *(CollisionCircle*)s2, result);
				}
				else if (st2 == CollisionShape::Box)
				{
					return Collider::intersectCircleBox(*t1, *(CollisionCircle*)s1, *t2, *(CollisionBox*)s2, result);
				}
			}
			else if (st1 == CollisionShape::Box)
			{
				if (st2 == CollisionShape::Circle)
				{
					float r = Collider::intersectCircleBox(*t2, *(CollisionCircle*)s2, *t1, *(CollisionBox*)s1, result);
					result->normal = -result->normal;
					return r;
				}
				else if (st2 == CollisionShape::Box)
				{
					return Collider::intersectBoxes(*t1, *(CollisionBox*)s1, *t2, *(CollisionBox*)s2, result);
				}
			}
		}
		return false;
	}

	void LynxEngine::separateBodies(RigidBody* b1, RigidBody* b2, CollisionResult result)
	{
		float inv_mass_sum = b1->getInverseMass() + b2->getInverseMass();
		if (inv_mass_sum > 0.f)
		{
			b1->move(-result.normal * result.depth * (b1->getInverseMass() / inv_mass_sum));
			b2->move(result.normal * result.depth * (b2->getInverseMass() / inv_mass_sum));
		}
	}

	void LynxEngine::resolveCollision(RigidBody* b1, RigidBody* b2, CollisionResult result)
	{
		float inv_mass_sum = b1->getInverseMass() + b2->getInverseMass();
		if (inv_mass_sum > 0.f)
		{
			float rel_v = LynxMath::dot(b1->getLinearVelocity() - b2->getLinearVelocity(), result.normal);
			float e = fminf(b1->getRestitution(), b2->getRestitution());
			Vector2 impulse = (-(1 + e) * rel_v / inv_mass_sum) * result.normal;

			b1->setLinearVelocity(b1->getLinearVelocity() + impulse * b1->getInverseMass());
			b2->setLinearVelocity(b2->getLinearVelocity() - impulse * b2->getInverseMass());
		}
	}

}