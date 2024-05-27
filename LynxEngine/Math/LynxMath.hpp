#pragma once
#include "Engine/Types.hpp"

namespace lynx
{
	class LynxMath
	{
	public:
		static float magnitudeSq(Vector2 vector);
		static float magnitude(Vector2 vector);
		static float distanceSq(Vector2 a, Vector2 b);
		static float distance(Vector2 a, Vector2 b);
		static Vector2 normilize(Vector2 vector);
		static float dot(Vector2 a, Vector2 b);
		static float cross(Vector2 a, Vector2 b);
		static float clamp(float value, float min, float max);
	};
}