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

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) body->setRotation((float)(rand() % 360));

		body->setInverseMass(1.f);
		body->setCollisionShape(shape);
		body->setRestitution(.2f);
		scene.addBody(body);
	});

	lynx::RigidBody* ground = new lynx::RigidBody();
	ground->setCollisionShape(new lynx::CollisionBox({200.f, 20.f}));
	ground->setPosition(200.f, 400.f);
	scene.addBody(ground);

	lynx::RigidBody* obstacle = new lynx::RigidBody();
	obstacle->setCollisionShape(new lynx::CollisionBox({ 80.f, 20.f }));
	obstacle->setPosition(100.f, 200.f);
	obstacle->setRotation(15.f);
	scene.addBody(obstacle);

	lynx::RigidBody* obstacle1 = new lynx::RigidBody();
	obstacle1->setCollisionShape(new lynx::CollisionBox({ 100.f, 20.f }));
	obstacle1->setPosition(350.f, 330.f);
	obstacle1->setRotation(-50.f);
	scene.addBody(obstacle1);

	lynx::RigidBody* obstacle2 = new lynx::RigidBody();
	obstacle2->setCollisionShape(new lynx::CollisionBox({ 20.f, 50.f }));
	obstacle2->setPosition(0.f, 330.f);
	scene.addBody(obstacle2);

	scene.enableGravity(true);

	// Main cycle
	while (window->isOpen())
	{
		engine->step(engine->getStepTime() * 10.f);
	}

	for (lynx::RigidBody* body : scene.getBodies())
	{
		delete body->getCollisionShape();
		delete body;
	}

	engine->shutDown();
	return 0;
}