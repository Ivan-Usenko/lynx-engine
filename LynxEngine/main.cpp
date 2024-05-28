
#include "Engine/LynxWindow.hpp"
#include "Physics/Collision/Collider.hpp"
#include "Math/LynxMath.hpp"

int main()
{
	lynx::LynxWindow window;
	window.setFramerateLimit(120.f);

	lynx::CollisionBox b1(lynx::Vector2(50.f, 30.f));
	lynx::Transform t1;
	t1.setPosition(200.f, 200.f);
	t1.setRotation(30.f);

	lynx::CollisionCircle c2(30.f);
	lynx::Transform t2;

	while (window.isOpen())
	{
		window.handleEvents();

		lynx::Vector2 mp = window.getRelMousePos();
		t2.setPosition(mp);

		window.clear();
		lynx::CollisionResult result;
		sf::Color color = sf::Color::White;
		if (lynx::Collider::intersect(t1, b1, t2, c2, &result))
		{
			color = sf::Color::Red;
			window.drawCircle(t2.getPosition().x, t2.getPosition().y, c2.getRadius(), sf::Color(0, 255, 0, 127));
			t2.move(result.normal * result.depth);
		}

		window.drawRectangle(t1.getPosition().x, t1.getPosition().y, b1.getSize().x, b1.getSize().y, t1.getRotation(), color);
		window.drawCircle(t2.getPosition().x, t2.getPosition().y, c2.getRadius(), color);
		window.display();
	}

	return 0;
}