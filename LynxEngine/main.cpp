#include "Engine/LynxEngine.hpp"
#include <random>

int main()
{
	lynx::LynxEngine* engine = lynx::LynxEngine::getLynxEngine();
	lynx::Scene scene;
	engine->setCurrentScene(&scene);

	lynx::LynxWindow* window = engine->getWindow();
	window->addEventHandler(sf::Event::MouseButtonPressed, [&](sf::Event e) {
		srand(time(0));
		lynx::RigidBody* body = new lynx::RigidBody();
		body->setPosition(window->getRelMousePos());
		lynx::CollisionShape* shape = nullptr;

		if (e.mouseButton.button == sf::Mouse::Left)
			shape = new lynx::CollisionBox({ (float)(rand() % 10 + 20), (float)(rand() % 10 + 20) });
		else if (e.mouseButton.button == sf::Mouse::Right) 
			shape = new lynx::CollisionCircle((float)(rand() % 10 + 20));

		body->setInverseMass(1.f);
		body->setCollisionShape(shape);
		body->setRestitution(.66f);
		scene.addBody(body);
	});

	lynx::RigidBody* ground = new lynx::RigidBody();
	ground->setCollisionShape(new lynx::CollisionBox({200.f, 20.f}));
	ground->setPosition(200.f, 400.f);
	scene.addBody(ground);

	scene.enableGravity(true);

	float f_mag = 600.f;

	// Main cycle
	while (window->isOpen())
	{
		engine->step(1.f / 60.f);
	}

	for (lynx::RigidBody* body : scene.getBodies())
	{
		delete body->getCollisionShape();
		delete body;
	}

	engine->shutDown();
	return 0;
}