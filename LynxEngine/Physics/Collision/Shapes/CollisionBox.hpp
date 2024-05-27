#pragma once
#include "CollisionShape.hpp"
#include "Engine/Types.hpp"

namespace lynx
{
	class CollisionBox : public CollisionShape
	{
	public:
		CollisionBox(Vector2 size = { 1.f, 1.f });
		void setSize(Vector2 size);
		Vector2 getSize();
		void calcBoxVertices(Vector2 vertices[4], Transform t);
	private:
		Vector2 m_size;
	};
}