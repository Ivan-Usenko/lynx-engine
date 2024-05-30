#pragma once
#include "Engine/Types.hpp"

namespace lynx
{
	class AABB
	{
	public:
		AABB();
		AABB(float min_x, float min_y, float max_x, float max_y);
		void setMin(Vector2 min);
		void setMax(Vector2 max);
		Vector2 getMin();
		Vector2 getMax();
	private:
		Vector2 m_min;
		Vector2 m_max;
	};
}