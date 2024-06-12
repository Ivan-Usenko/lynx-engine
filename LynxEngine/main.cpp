#include "Engine/LynxEngine.hpp"
#include <random>
#include "Math/LynxMath.hpp"
#include <iomanip>
#include <sstream>

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
		body->setFriction(0.3f);
		scene.addBody(body);
	});

	lynx::RigidBody* ground = new lynx::RigidBody();
	ground->setCollisionShape(new lynx::CollisionBox({400.f, 20.f}));
	ground->setPosition(400.f, 600.f);
	scene.addBody(ground);

	lynx::RigidBody* obstacle = new lynx::RigidBody();
	obstacle->setCollisionShape(new lynx::CollisionBox({ 20.f, 300.f }));
	obstacle->setPosition(0.f, 300.f);
	scene.addBody(obstacle);

	lynx::RigidBody* obstacle1 = new lynx::RigidBody();
	obstacle1->setCollisionShape(new lynx::CollisionBox({ 20.f, 300.f }));
	obstacle1->setPosition(800.f, 300.f);
	scene.addBody(obstacle1);

	sf::Texture texture;
	texture.loadFromFile("test.png");
	sf::Sprite sprite;
	sprite.setTexture(texture);

	lynx::RigidBody* player = new lynx::RigidBody();
	player->setCollisionShape(new lynx::CollisionBox({ 25.f, 25.f }));
	player->setPosition(100.f, 150.f);
	player->setInverseMass(1.f);
	player->setRestitution(.2f);
	player->setSprite(&sprite);
	scene.addBody(player);

	engine->enableDebugMode(true);

	float player_speed = 80.f;
	float jump_force = 50.f;

	window->addEventHandler(sf::Event::KeyPressed, [&](sf::Event e)
		{
			if (e.key.code == sf::Keyboard::Space)
			{
				player->applyImpulse(lynx::Vector2(0.f, -1.f) * jump_force);
			}
		});

	engine->attachDrawingProc([&](lynx::LynxWindow* window)
		{
			engine->drawBodies();
	std::stringstream ss;
	ss << "Time step: " << std::fixed << std::setprecision(4) << engine->getStepTime() << std::endl;
	ss << "Body count: " << scene.getBodies().size();
	window->drawLabel(ss.str(), 15u);
		});

	// Main cycle
	while (window->isOpen())
	{
		float dt = engine->getStepTime();
		lynx::Vector2 p_vel = player->getLinearVelocity();
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) player->setLinearVelocity(p_vel + lynx::Vector2(-1.f, 0.f) * player_speed * dt);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) player->setLinearVelocity(p_vel + lynx::Vector2(1.f, 0.f) * player_speed * dt);

		engine->step(dt * 10.f);
	}

	for (lynx::RigidBody* body : scene.getBodies())
	{
		delete body->getCollisionShape();
		delete body;
	}

	engine->shutDown();
	return 0;
}