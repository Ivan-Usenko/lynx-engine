#include "CollisionCircle.hpp"

namespace lynx
{
	CollisionCircle::CollisionCircle(float radius) : CollisionShape(Circle)
	{
		m_radius = radius;
	}

	void CollisionCircle::setRadius(float radius)
	{
		m_radius = radius;
	}

	float CollisionCircle::getRadius()
	{
		return m_radius;
	}

	float CollisionCircle::getInvInertiaCoef()
	{
		return 2.f / (m_radius * m_radius);
	}
}