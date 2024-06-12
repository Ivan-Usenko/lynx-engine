#include "CollisionShape.hpp"

namespace lynx
{
	CollisionShape::CollisionShape(ShapeType type)
	{
		m_type = type;
	}


	CollisionShape::ShapeType CollisionShape::getType()
	{
		return m_type;
	}
}