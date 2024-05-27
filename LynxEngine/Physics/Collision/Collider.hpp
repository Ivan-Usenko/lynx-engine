#pragma once
#include "Physics/Collision/CollisionShapes.hpp"
#include "Engine/Types.hpp"

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
		static bool intersect(Transform t1, CollisionCircle c1, Transform t2, CollisionCircle c2, CollisionResult* result);
	};
}