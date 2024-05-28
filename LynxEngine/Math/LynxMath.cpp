#include "LynxMath.hpp"
#define _USE_MATH_DEFINES
#include <math.h>
#include <limits.h>

namespace lynx
{

	float LynxMath::magnitudeSq(Vector2 vector)
	{
		return vector.x * vector.x + vector.y * vector.y;
	}

	float LynxMath::magnitude(Vector2 vector)
	{
		return sqrt(magnitudeSq(vector));
	}

	float LynxMath::distanceSq(Vector2 a, Vector2 b)
	{
		return magnitudeSq(a - b);
	}

	float LynxMath::distance(Vector2 a, Vector2 b)
	{
		return magnitude(a - b);
	}

	Vector2 LynxMath::normilize(Vector2 vector)
	{
		return vector / magnitude(vector);
	}

	float LynxMath::dot(Vector2 a, Vector2 b)
	{
		return a.x * b.x + a.y * b.y;
	}

	float LynxMath::cross(Vector2 a, Vector2 b)
	{
		return a.x * b.y - a.y * b.x;
	}

	float LynxMath::clamp(float value, float min, float max)
	{
		return fmaxf(min, fminf(value, max));
	}

	Vector2 LynxMath::clamp(Vector2 value, Vector2 min, Vector2 max)
	{
		return Vector2(clamp(value.x, min.x, max.x), clamp(value.y, min.y, max.y));
	}

	Vector2 LynxMath::abs(Vector2 vector)
	{
		return Vector2(fabsf(vector.x), fabsf(vector.y));
	}

	Vector2 LynxMath::rotate(Vector2 vector, float angle, Vector2 origin)
	{
		float ang_rad = toRadians(angle);
		float sin_a = sinf(ang_rad);
		float cos_a = cosf(ang_rad);

		vector -= origin;
		Vector2 rotated_vector;
		rotated_vector.x = vector.x * cos_a - vector.y * sin_a + origin.x;
		rotated_vector.y = vector.x * sin_a + vector.y * cos_a + origin.y;

		return rotated_vector;
	}

	float LynxMath::toRadians(float angle)
	{
		return angle * M_PI * 0.00555555555f;
	}

	float LynxMath::toDegrees(float angle)
	{
		return angle * 180.f * M_1_PI;
	}

	bool LynxMath::equalf(float f1, float f2, float e)
	{
		return fabsf(f1 - f2) < e;
	}

	bool LynxMath::equalf(Vector2 v1, Vector2 v2, float e)
	{
		return equalf(v1.x, v2.x, e) && equalf(v1.y, v2.y, e);
	}
}