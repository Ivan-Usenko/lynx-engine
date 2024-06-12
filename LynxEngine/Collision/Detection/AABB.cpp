#include "AABB.hpp"

namespace lynx
{
	AABB::AABB() { }

	AABB::AABB(float min_x, float min_y, float max_x, float max_y)
	{
		m_min = Vector2(min_x, min_y);
		m_max = Vector2(max_x, max_y);
	}

	void AABB::setMin(Vector2 min)
	{
		m_min = min;
	}

	void AABB::setMax(Vector2 max)
	{
		m_max = max;
	}

	Vector2 AABB::getMin()
	{
		return m_min;
	}

	Vector2 AABB::getMax()
	{
		return m_max;
	}
}