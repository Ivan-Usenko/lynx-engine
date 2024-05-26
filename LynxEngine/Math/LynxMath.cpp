#include "LynxMath.hpp"
#define _USE_MATH_DEFINES
#include <math.h>

namespace lynx
{

	float LynxMath::magnitudeSq(sf::Vector2f vector)
	{
		return vector.x * vector.x + vector.y * vector.y;
	}

	float LynxMath::magnitude(sf::Vector2f vector)
	{
		return sqrt(magnitudeSq(vector));
	}

	float LynxMath::distanceSq(sf::Vector2f a, sf::Vector2f b)
	{
		return magnitudeSq(a - b);
	}

	float LynxMath::distance(sf::Vector2f a, sf::Vector2f b)
	{
		return magnitude(a - b);
	}

	sf::Vector2f LynxMath::normilize(sf::Vector2f vector)
	{
		return vector / magnitude(vector);
	}

	float LynxMath::dot(sf::Vector2f a, sf::Vector2f b)
	{
		return a.x * b.x + a.y * b.y;
	}

	float LynxMath::cross(sf::Vector2f a, sf::Vector2f b)
	{
		return a.x * b.y - a.y * b.x;
	}

	float LynxMath::clamp(float value, float min, float max)
	{
		return fmaxf(min, fminf(value, max));
	}
}