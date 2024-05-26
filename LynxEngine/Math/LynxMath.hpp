#pragma once
#include <SFML/System/Vector2.hpp>

namespace lynx
{
	class LynxMath
	{
	public:
		static float magnitudeSq(sf::Vector2f vector);
		static float magnitude(sf::Vector2f vector);
		static float distanceSq(sf::Vector2f a, sf::Vector2f b);
		static float distance(sf::Vector2f a, sf::Vector2f b);
		static sf::Vector2f normilize(sf::Vector2f vector);
		static float dot(sf::Vector2f a, sf::Vector2f b);
		static float cross(sf::Vector2f a, sf::Vector2f b);
		static float clamp(float value, float min, float max);
	};
}