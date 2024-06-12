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
		virtual float getInvInertiaCoef() = 0;
	private:
		ShapeType m_type;
	};
}

