#include "Scene.hpp"

namespace lynx
{
	Scene::Scene()
	{
		m_gravity_enabled = true;
		m_gravity = Vector2(0.f, 9.8f);
	}

	void Scene::setGravity(Vector2 gravity)
	{
		m_gravity = gravity;
	}

	Vector2 Scene::getGravity()
	{
		return m_gravity;
	}

	void Scene::enableGravity(bool enable)
	{
		m_gravity_enabled = enable;
	}

	bool Scene::isGravityEnabled()
	{
		return m_gravity_enabled;
	}

	void Scene::addBody(RigidBody* body)
	{
		m_bodies.push_back(body);
	}

	void Scene::removeBody(RigidBody* body)
	{
		m_bodies.remove(body);
	}

	const std::list<RigidBody*>& Scene::getBodies()
	{
		return m_bodies;
	}

	void Scene::clearBoies()
	{
		m_bodies.clear();
	}
}