#pragma once
#include "Physics/Collision/CollisionShapes.hpp"
#include "Engine/Types.hpp"
#include "Engine/LynxWindow.hpp"
#include "AABB.hpp"
#include "Physics/RigidBody.hpp"

namespace lynx
{
	struct CollisionResult
	{
		RigidBody* body1;
		RigidBody* body2;
		Vector2 normal;
		float depth;
		Vector2 contact[2];
		int contact_count;
	};

	class Collider
	{
	public:
		static bool isBodiesCollide(RigidBody* b1, RigidBody* b2, CollisionResult* result);
		static bool intersectCircles(Transform* t1, CollisionCircle* c1, Transform* t2, CollisionCircle* c2, CollisionResult* result);
		static bool intersectBoxes(Transform* t1, CollisionBox* b1, Transform* t2, CollisionBox* b2, CollisionResult* result);
		static bool intersectCircleBox(Transform* t1, CollisionCircle* c1, Transform* t2, CollisionBox* b2, CollisionResult* result);
		static bool isAABBsOverlap(AABB* b1, AABB* b2);
		static void findContactPoints(CollisionResult* result);
	private:
		static void calcMinAndMaxProjections(Vector2* vertices, int v_count, Vector2 axis, float* min, float* max);
		static float projectPointOntoEdge(Vector2 p, Vector2 v1, Vector2 v2, Vector2* cp);
		static void findClosestVertexToBox(Vector2* b1_vertices, Vector2* b2_vertices, float* min_dist_sq, CollisionResult* result);

		static void findContactPointsCC(Transform* t1, CollisionCircle* c1, CollisionResult* result);
		static void findContactPointsBB(Transform* t1, CollisionBox* b1, Transform* t2, CollisionBox* b2, CollisionResult* result);
		static void findContactPointsCB(Transform* t1, CollisionCircle* c1, Transform* t2, CollisionBox* b2, CollisionResult* result);

		Collider() {};
	};
}