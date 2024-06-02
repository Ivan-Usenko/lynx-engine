#include "CollisionBox.hpp"


namespace lynx
{
	CollisionBox::CollisionBox(Vector2 size) : CollisionShape(Box)
	{
		m_size = size;
	}

	void CollisionBox::setSize(Vector2 size)
	{
		m_size = size;
	}

	Vector2 CollisionBox::getSize()
	{
		return m_size;
	}

	void CollisionBox::calcBoxVertices(Vector2 vertices[4], Transform* t)
	{
		sf::Transform mx = t->getTransform();
		vertices[0] = mx.transformPoint(-m_size);
		vertices[1] = mx.transformPoint(m_size.x, -m_size.y);
		vertices[2] = mx.transformPoint(m_size);
		vertices[3] = mx.transformPoint(-m_size.x, m_size.y);
	}

	float CollisionBox::getInvInertiaCoef()
	{
		return 12.f / (m_size.x * m_size.x + m_size.y * m_size.y);
	}
}