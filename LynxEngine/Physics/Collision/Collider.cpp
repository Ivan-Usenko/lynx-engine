#include "Collider.hpp"
#include "Math/LynxMath.hpp"

namespace lynx
{
	bool Collider::intersect(Transform t1, CollisionCircle c1, Transform t2, CollisionCircle c2, CollisionResult* result)
	{
		sf::Vector2f dir = t2.getPosition() - t1.getPosition();
		float dist_sq = LynxMath::magnitudeSq(dir);
		float rad_sum = c1.getRadius() + c2.getRadius();

		if (dist_sq < rad_sum * rad_sum)
		{
			if (result)
			{
				result->normal = LynxMath::normilize(dir);
				result->depth = rad_sum - sqrtf(dist_sq);
			}
			return true;
		}
		return false;
	}
}