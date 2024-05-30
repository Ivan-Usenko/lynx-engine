#include "Collider.hpp"
#include "Math/LynxMath.hpp"

namespace lynx
{
	bool Collider::intersectCircles(Transform t1, CollisionCircle c1, Transform t2, CollisionCircle c2, CollisionResult* result)
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

	bool Collider::intersectBoxes(Transform t1, CollisionBox b1, Transform t2, CollisionBox b2, CollisionResult* result)
	{
		Vector2 b1_vertices[4];
		Vector2 b2_vertices[4];
		b1.calcBoxVertices(b1_vertices, t1);
		b2.calcBoxVertices(b2_vertices, t2);

		Vector2 min_axis;
		float min_depth = std::numeric_limits<float>::max();
		for (int i = 0; i < 4; i++)
		{
			Vector2 edge = b1_vertices[(i + 1) % 4] - b1_vertices[i];
			Vector2 axis = LynxMath::normilize(Vector2(-edge.y, edge.x));
			
			float min1, max1, min2, max2;
			calcMinAndMaxProjections(b1_vertices, 4, axis, &min1, &max1);
			calcMinAndMaxProjections(b2_vertices, 4, axis, &min2, &max2);

			if (min1 >= max2 || min2 >= max1) return false;
			float depth = fminf(max2 - min1, max1 - min2);

			if (depth < min_depth)
			{
				min_depth = depth;
				min_axis = axis;
			}
		}

		for (int i = 0; i < 4; i++)
		{
			Vector2 edge = b2_vertices[(i + 1) % 4] - b2_vertices[i];
			Vector2 axis = LynxMath::normilize(Vector2(-edge.y, edge.x));

			float min1, max1, min2, max2;
			calcMinAndMaxProjections(b1_vertices, 4, axis, &min1, &max1);
			calcMinAndMaxProjections(b2_vertices, 4, axis, &min2, &max2);

			if (min1 >= max2 || min2 >= max1) return false;
			float depth = fminf(max2 - min1, max1 - min2);

			if (depth < min_depth)
			{
				min_depth = depth;
				min_axis = axis;
			}
		}

		if (result)
		{
			if (LynxMath::dot(t2.getPosition() - t1.getPosition(), min_axis) < 0.f) min_axis = -min_axis;
			result->normal = min_axis;
			result->depth = min_depth;
		}

		return true;
	}

	bool Collider::intersectCircleBox(Transform t1, CollisionCircle c1, Transform t2, CollisionBox b2, CollisionResult* result)
	{
		Vector2 c_loc_pos = LynxMath::rotate(t1.getPosition(), -t2.getRotation(), t2.getPosition());
		Vector2 min = t2.getPosition() - b2.getSize();
		Vector2 max = t2.getPosition() + b2.getSize();

		Vector2 cp = LynxMath::clamp(c_loc_pos, min, max);
		Vector2 dir = cp - c_loc_pos;
		float dist_sq = LynxMath::magnitudeSq(dir);
		if (dist_sq < c1.getRadius() * c1.getRadius())
		{
			if (result)
			{
				// If circle center inside the box
				if (LynxMath::equalf(dir, Vector2()))
				{
					Vector2 d = t2.getPosition() - cp;
					Vector2 coll_reg = b2.getSize() - LynxMath::abs(d);
					Vector2 axis = coll_reg.x < coll_reg.y ? Vector2(1.f, 0.f) : Vector2(0.f, 1.f);
					dir = axis * LynxMath::dot(d, axis);
					result->depth = c1.getRadius() + fminf(coll_reg.x, coll_reg.y);
				}
				else result->depth = c1.getRadius() - sqrtf(dist_sq);

				result->normal = LynxMath::rotate(LynxMath::normilize(dir), t2.getRotation());
			}

			return true;
		}

		return false;
	}

	void Collider::calcMinAndMaxProjections(Vector2* vertices, int v_count, Vector2 axis, float* min, float* max)
	{
		*min = std::numeric_limits<float>::max();
		*max = std::numeric_limits<float>::min();

		for (int i = 0; i < v_count; i++)
		{
			float proj = LynxMath::dot(vertices[i], axis);
			*min = fminf(*min, proj);
			*max = fmaxf(*max, proj);
		}
	}
}