#pragma once
#include "Physics/Collision/CollisionShapes.hpp"
#include "Engine/Types.hpp"
#include "Engine/LynxWindow.hpp"
#include "AABB.hpp"

namespace lynx
{
	struct CollisionResult
	{
		sf::Vector2f normal;
		float depth;
	};

	class Collider
	{
	public:
		static bool intersectCircles(Transform t1, CollisionCircle c1, Transform t2, CollisionCircle c2, CollisionResult* result);
		static bool intersectBoxes(Transform t1, CollisionBox b1, Transform t2, CollisionBox b2, CollisionResult* result);
		static bool intersectCircleBox(Transform t1, CollisionCircle c1, Transform t2, CollisionBox b2, CollisionResult* result);
		static bool isAABBOverlap(AABB b1, AABB b2);
	private:
		static void calcMinAndMaxProjections(Vector2* vertices, int v_count, Vector2 axis, float* min, float* max);
	};
}