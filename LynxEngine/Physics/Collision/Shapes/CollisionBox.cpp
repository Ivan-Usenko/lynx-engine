#include "CollisionBox.hpp"

namespace lynx
{
	CollisionBox::CollisionBox(sf::Vector2f size) : CollisionShape(Box)
	{
		m_size = size;
	}

	void CollisionBox::setSize(sf::Vector2f size)
	{
		m_size = size;
	}

	sf::Vector2f CollisionBox::getSize()
	{
		return m_size;
	}
}