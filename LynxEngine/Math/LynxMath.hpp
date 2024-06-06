#pragma once
#include "Engine/Types.hpp"

#define	DEFAULT_EPSILON		0.001f

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
		static Vector2 clamp(Vector2 value, Vector2 min, Vector2 max);
		static Vector2 abs(Vector2 vector);
		static Vector2 rotate(Vector2 vector, float angle, Vector2 origin = Vector2());
		static float toRadians(float angle);
		static float toDegrees(float angle);
		static bool equalf(float f1, float f2, float e = DEFAULT_EPSILON);
		static bool equalf(Vector2 v1, Vector2 v2, float e = DEFAULT_EPSILON);
		static float roundf(float x, int precision);
	private:
		LynxMath() {};
	};
}