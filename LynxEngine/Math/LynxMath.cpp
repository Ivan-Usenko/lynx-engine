#include "LynxMath.hpp"
#define _USE_MATH_DEFINES
#include <math.h>

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
}