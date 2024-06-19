#include "Engine/LynxEngine.hpp"
#include "Math/LynxMath.hpp"
#include "Collision/Detection/Collider.hpp"
#include <iomanip>
#include <sstream>

lynx::Scene& curScene()
{
	return *lynx::LynxEngine::getLynxEngine()->getCurrentScene();
}

lynx::LynxWindow* mainWindow()
{
	return lynx::LynxEngine::getLynxEngine()->getWindow();
}

void freeSceneMemory()
{
	for (lynx::RigidBody* body : curScene().getBodies())
	{
		delete body->getCollisionShape();
		delete body;
	}
}

void reloadLevel(lynx::RigidBody*& player)
{
	lynx::Scene& scene = curScene();
	freeSceneMemory();
	if (player && player->getSprite())
	{
		delete player->getSprite()->getTexture();
		delete player->getSprite();
	}
	scene.clearBoies();

	// Ground
	lynx::RigidBody* ground = new lynx::RigidBody();
	ground->setCollisionShape(new lynx::CollisionBox(lynx::Vector2(600.f, 20.f)));
	ground->setPosition(800.f, 750.f);
	scene.addBody(ground);

	// Player
	sf::Texture* p_texture = new sf::Texture();
	p_texture->loadFromFile("player.png");
	player = new lynx::RigidBody();
	player->setCollisionShape(new lynx::CollisionBox(lynx::Vector2(25.f, 25.f)));
	player->setPosition(500.f, 700.f);
	player->setInverseMass(1.f);
	player->setRestitution(0.2f);
	player->setSprite(new sf::Sprite(*p_texture));
	scene.addBody(player);

	// Obstacle #1
	lynx::RigidBody* obstacle1 = new lynx::RigidBody();
	obstacle1->setCollisionShape(new lynx::CollisionBox(lynx::Vector2(20.f, 50.f)));
	obstacle1->setPosition(700.f, 680.f);
	scene.addBody(obstacle1);

	// Obstacle #2
	lynx::RigidBody* obstacle2 = new lynx::RigidBody();
	obstacle2->setCollisionShape(new lynx::CollisionBox(lynx::Vector2(200.f, 20.f)));
	obstacle2->setPosition(880.f, 610.f);
	scene.addBody(obstacle2);

	// Obstacle #3
	lynx::RigidBody* obstacle3 = new lynx::RigidBody();
	obstacle3->setCollisionShape(new lynx::CollisionBox(lynx::Vector2(100.f, 20.f)));
	obstacle3->setPosition(1135.f, 675.f);
	obstacle3->setRotation(45.f);
	scene.addBody(obstacle3);

	// Obstacle #4
	lynx::RigidBody* obstacle4 = new lynx::RigidBody();
	obstacle4->setCollisionShape(new lynx::CollisionBox(lynx::Vector2(200.f, 20.f)));
	obstacle4->setPosition(880.f, 450.f);
	scene.addBody(obstacle4);

	// Ball
	lynx::RigidBody* ball = new lynx::RigidBody();
	ball->setCollisionShape(new lynx::CollisionCircle(50.f));
	ball->setPosition(880.f, 575.f);
	ball->setInverseMass(0.1f);
	ball->setRestitution(0.2f);
	scene.addBody(ball);

	// Box
	lynx::RigidBody* box = new lynx::RigidBody();
	box->setCollisionShape(new lynx::CollisionBox(lynx::Vector2(25.f, 35.f)));
	box->setPosition(400.f, 700.f);
	box->setInverseMass(1.f);
	box->setRestitution(0.2f);
	scene.addBody(box);
}

void playerControl(lynx::RigidBody*& player, float dt)
{
	float speed = 5.f;
	float jump_force = 50.f;
	static bool last_space_state = false;

	// Jumping
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && !last_space_state)
	{
		player->applyImpulse(lynx::Vector2(0.f, -jump_force));
		last_space_state = true;
	}
	else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && last_space_state)
	{
		last_space_state = false;
	}

	// Walking
	lynx::Vector2 l_vel = player->getLinearVelocity();
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) player->setLinearVelocity(l_vel + lynx::Vector2(-speed, 0.f) * dt);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) player->setLinearVelocity(l_vel + lynx::Vector2(speed, 0.f) * dt);
}

int main()
{
	lynx::LynxEngine* engine = lynx::LynxEngine::getLynxEngine();
	lynx::Scene scene;
	engine->setCurrentScene(&scene);
	lynx::LynxWindow* window = mainWindow();
	window->create(sf::VideoMode(1600, 800), "Lynx Engine Demonstration");
	
	engine->enableDebugMode(true);

	lynx::RigidBody* player = nullptr;
	reloadLevel(player);

	// Coin
	lynx::RigidBody coin;
	coin.setCollisionShape(new lynx::CollisionCircle(15.f));
	coin.setPosition(1350.f, 680.f);

	std::string game_state_text = "";
	bool is_win = false;
	bool is_game_run = true;
	engine->attachDrawingProc([&](lynx::LynxWindow* window)
		{
			engine->drawBodies();
			window->drawCircle(coin.getPosition().x, coin.getPosition().y,
				((lynx::CollisionCircle*)coin.getCollisionShape())->getRadius(), 0.f, sf::Color::White, sf::Color::Yellow);
			std::stringstream ss;
			ss << "Time step: " << std::fixed << std::setprecision(4) << engine->getStepTime() << std::endl;
			ss << "Body count: " << scene.getBodies().size();
			window->drawLabel(ss.str(), 15u);
			if (!is_game_run)
				window->drawLabel(game_state_text, 30u, lynx::Vector2(300.f, 300.f), is_win ? sf::Color::Green : sf::Color::Red);
		});

	// Main cycle
	while (window->isOpen())
	{
		float dt = is_game_run ? engine->getStepTime() * 10.f : 0.f;
		playerControl(player, dt);
		engine->step(dt);

		if (is_game_run)
		{
			// Player falls off from the ground
			if (player->getPosition().y > 800.f)
			{
				is_game_run = false;
				game_state_text = "You lost!\nPress R to restart.";
				is_win = false;
			}

			// Player take the coin
			if (lynx::Collider::isBodiesCollide(player, &coin, nullptr))
			{
				is_game_run = false;
				game_state_text = "You won!\nPress R to restart.";
				is_win = true;
			}
		}
		else if (!is_game_run && sf::Keyboard::isKeyPressed(sf::Keyboard::R))
		{
			game_state_text = "";
			is_game_run = true;
			is_win = false;
			reloadLevel(player);
		}
	}

	freeSceneMemory();
	engine->shutDown();
	return 0;
}