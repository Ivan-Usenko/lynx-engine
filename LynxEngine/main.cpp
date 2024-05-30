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
		scene.addBody(body);
	});

	lynx::RigidBody* player = new lynx::RigidBody();
	player->setCollisionShape(new lynx::CollisionBox({ 30.f, 30.f }));
	player->setPosition(200.f, 200.f);
	player->setInverseMass(1.f);
	scene.addBody(player);

	lynx::RigidBody* ground = new lynx::RigidBody();
	ground->setCollisionShape(new lynx::CollisionBox({200.f, 20.f}));
	ground->setPosition(200.f, 400.f);
	scene.addBody(ground);

	scene.enableGravity(false);

	float f_mag = 600.f;

	// Main cycle
	while (window->isOpen())
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) player->addForce(f_mag * lynx::Vector2(0.f, -1.f));
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) player->addForce(f_mag * lynx::Vector2(0.f, 1.f));
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) player->addForce(f_mag * lynx::Vector2(-1.f, 0.f));
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) player->addForce(f_mag * lynx::Vector2(1.f, 0.f));

		engine->step(1.f / 60.f);
		player->clearForce();
	}

	for (lynx::RigidBody* body : scene.getBodies())
	{
		delete body->getCollisionShape();
		delete body;
	}

	engine->shutDown();
	return 0;
}