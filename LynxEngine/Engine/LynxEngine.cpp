#include "LynxEngine.hpp"
#include "Math/LynxMath.hpp"
#include <iomanip>
#include <sstream>

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

		std::list<Vector2> contacts;
		integrateAccel(dt);
		integrateVelocity(dt);

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
					CollisionResult result;
					if (Collider::isBodiesCollide(b1, b2, &result))
					{
						separateBodies(result);
						Collider::findContactPoints(&result);

						for (int c = 0; c < result.contact_count; c++) contacts.push_back(result.contact[c]);

						resolveCollision(result);
					}
				}
			}
		}

		m_window.clear();
		drawBodies();
		for (Vector2 c : contacts) m_window.drawCircle(c.x, c.y, 3.f, sf::Color::White, sf::Color::Red);
		drawInterface();
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

	void LynxEngine::drawInterface()
	{
		std::stringstream ss;
		ss << "DEBUG\n";
		ss << "Time step: " << std::fixed << std::setprecision(4) << m_clock.restart().asSeconds() << std::endl;
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