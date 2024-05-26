#pragma once
#include "CollisionShape.hpp"
#include <SFML/System/Vector2.hpp>

namespace lynx
{
	class CollisionBox : public CollisionShape
	{
	public:
		CollisionBox(sf::Vector2f size = { 1.f, 1.f });
		void setSize(sf::Vector2f size);
		sf::Vector2f getSize();
	private:
		sf::Vector2f m_size;
	};
}