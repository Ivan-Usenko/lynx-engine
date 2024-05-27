
#include "Engine/LynxWindow.hpp"
#include "Physics/Collision/Collider.hpp"

int main()
{
	lynx::LynxWindow window;
	window.setFramerateLimit(120.f);

	lynx::CollisionBox c1(lynx::Vector2(50.f, 30.f));
	lynx::Transform t1;
	t1.setPosition(200.f, 200.f);
	t1.setRotation(30.f);

	while (window.isOpen())
	{
		window.handleEvents();

		window.clear();
		window.drawRectangle(t1.getPosition().x, t1.getPosition().y, c1.getSize().x, c1.getSize().y, t1.getRotation());
		window.display();
	}

	return 0;
}