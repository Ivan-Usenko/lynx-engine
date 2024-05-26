#pragma once

namespace lynx
{
	class CollisionShape
	{
	public:
		enum ShapeType
		{
			Circle,
			Box
		};

		CollisionShape(ShapeType type);
		ShapeType getType();
	private:
		ShapeType m_type;
	};
}