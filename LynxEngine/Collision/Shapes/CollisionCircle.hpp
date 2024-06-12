#pragma once
#include "CollisionShape.hpp"

namespace lynx
{
	class CollisionCircle : public CollisionShape
	{
	public:
		CollisionCircle(float radius = 1.f);
		void setRadius(float radius);
		float getRadius();
		float getInvInertiaCoef() override;
	private:
		float m_radius;
	};
}