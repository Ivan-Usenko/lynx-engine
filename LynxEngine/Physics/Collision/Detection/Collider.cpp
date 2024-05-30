#include "Collider.hpp"
#include "Math/LynxMath.hpp"

namespace lynx
{

	bool Collider::isBodiesCollide(RigidBody* b1, RigidBody* b2, CollisionResult* result)
	{
		CollisionShape* s1 = b1->getCollisionShape();
		CollisionShape* s2 = b2->getCollisionShape();

		if (s1 && s2)
		{
			CollisionShape::ShapeType st1 = s1->getType();
			CollisionShape::ShapeType st2 = s2->getType();
			Transform* t1 = b1;
			Transform* t2 = b2;

			bool is_collide = false;
			if (st1 == CollisionShape::Circle)
			{
				if (st2 == CollisionShape::Circle)
				{
					is_collide = Collider::intersectCircles(*t1, *(CollisionCircle*)s1, *t2, *(CollisionCircle*)s2, result);
				}
				else if (st2 == CollisionShape::Box)
				{
					is_collide = Collider::intersectCircleBox(*t1, *(CollisionCircle*)s1, *t2, *(CollisionBox*)s2, result);
				}
			}
			else if (st1 == CollisionShape::Box)
			{
				if (st2 == CollisionShape::Circle)
				{
					float r = Collider::intersectCircleBox(*t2, *(CollisionCircle*)s2, *t1, *(CollisionBox*)s1, result);
					result->normal = -result->normal;
					is_collide = r;
				}
				else if (st2 == CollisionShape::Box)
				{
					is_collide = Collider::intersectBoxes(*t1, *(CollisionBox*)s1, *t2, *(CollisionBox*)s2, result);
				}
			}

			if (is_collide && result)
			{
				result->body1 = b1;
				result->body2 = b2;
			}

			return is_collide;
		}
		return false;
	}

	bool Collider::intersectCircles(Transform t1, CollisionCircle c1, Transform t2, CollisionCircle c2, CollisionResult* result)
	{
		Vector2 dir = t2.getPosition() - t1.getPosition();
		float dist_sq = LynxMath::magnitudeSq(dir);
		float rad_sum = c1.getRadius() + c2.getRadius();

		if (dist_sq < rad_sum * rad_sum)
		{
			if (result)
			{
				// If circles centers in the same position
				if (LynxMath::equalf(dir, Vector2())) result->normal = Vector2(0.f, -1.f);
				else result->normal = LynxMath::normilize(dir);
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

	bool Collider::isAABBOverlap(AABB b1, AABB b2)
	{
		float ov_right = fmaxf(b1.getMin().x, b2.getMin().x);
		float ov_bottom = fmaxf(b1.getMin().y, b2.getMin().y);
		float ov_left = fminf(b1.getMax().x, b2.getMax().x);
		float ov_top = fminf(b1.getMax().y, b2.getMax().y);

		return (ov_right > ov_left) && (ov_bottom > ov_top);
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

	void Collider::findContactPoints(CollisionResult* result)
	{
		RigidBody* b1 = result->body1;
		RigidBody* b2 = result->body2;
		result->contact_count = 0;

		if (b1 && b2)
		{
			CollisionShape* s1 = b1->getCollisionShape();
			CollisionShape* s2 = b2->getCollisionShape();

			if (s1 && s2)
			{
				CollisionShape::ShapeType type1 = s1->getType();
				CollisionShape::ShapeType type2 = s2->getType();

				if (type1 == CollisionShape::Circle)
				{
					if (type2 == CollisionShape::Circle)
					{
						findContactPointsCC(*(Transform*)b1, *(CollisionCircle*)s1, result);
					}
					else if (type2 == CollisionShape::Box)
					{
						findContactPointsCB(*(Transform*)b1, *(CollisionCircle*)s1, *(Transform*)b2, *(CollisionBox*)s2, result);
					}
				}
				else if (type1 == CollisionShape::Box)
				{
					if (type2 == CollisionShape::Circle)
					{
						findContactPointsCB(*(Transform*)b2, *(CollisionCircle*)s2, *(Transform*)b1, *(CollisionBox*)s1, result);
					}
					else if (type2 == CollisionShape::Box)
					{
						findContactPointsBB(*(Transform*)b1, *(CollisionBox*)s1, *(Transform*)b2, *(CollisionBox*)s2, result);
					}
				}
			}
		}
	}

	void Collider::findContactPointsCC(Transform t1, CollisionCircle c1, CollisionResult* result)
	{
		result->contact_count = 1;
		result->contact[0] = t1.getPosition() + result->normal * c1.getRadius();
	}

	void Collider::findContactPointsBB(Transform t1, CollisionBox b1, Transform t2, CollisionBox b2, CollisionResult* result)
	{

	}

	void Collider::findContactPointsCB(Transform t1, CollisionCircle c1, Transform t2, CollisionBox b2, CollisionResult* result)
	{

	}
}