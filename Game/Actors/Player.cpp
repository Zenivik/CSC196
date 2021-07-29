#include "Player.h"
#include "Enemy.h"
#include "Projectile.h"
#include "Engine.h"
#include <memory>

Player::Player(const nc::Transform& transform, std::shared_ptr<nc::Shape> shape, float speed) : nc::Actor{ transform, shape }, speed{ speed }
{
}

void Player::Initialize()
{
	std::unique_ptr locator = std::make_unique<Actor>();
	locator->transform.localPosition = nc::Vector2{ 8, 0 };
	locator->transform.localPosition = nc::DegToRad(180);
	AddChild(std::move(locator));

	locator = std::make_unique<Actor>();
	locator->transform.localPosition = nc::Vector2{ 0, 5 };
	AddChild(std::move(locator));

	/*locator = std::make_unique<Actor>();
	locator->transform.localPosition = nc::Vector2{ 0, -5 };
	AddChild(std::move(locator));*/

	std::unique_ptr engine = std::make_unique<Actor>(nc::Transform(), scene->engine->Get<nc::ResourceSystem>()->Get<nc::Shape>("Engine.txt"));
	engine->transform.localPosition = nc::Vector2{ -4.5f, 0.0f };
	engine->transform.scale *= 0.5f;
	AddChild(std::move(engine));
}

void Player::Update(float dt)
{
	//children[3]->transform.localRotation += 5 * dt;

	//movement
	float thrust = 0;
	if (Core::Input::IsPressed('A')) transform.rotation += -5 * dt;
	if (Core::Input::IsPressed('D')) transform.rotation += 5 * dt;
	if (Core::Input::IsPressed('W')) thrust = speed;

	nc::Vector2 acceleration;
	acceleration += nc::Vector2::Rotate(nc::Vector2::right, transform.rotation) * thrust;
	//nc::Vector2 gravity = (nc::Vector2{ 400, 300 } - transform.position).Normalized() * 100;
	/*nc::Vector2 gravity = nc::Vector2::down * 100;
	acceleration += gravity;*/

	velocity += acceleration * dt;
	transform.position += velocity * dt;

	velocity *= 0.985f;

	transform.position.x = nc::Wrap(transform.position.x, 0.0f, 800.0f);
	transform.position.y = nc::Wrap(transform.position.y, 0.0f, 600.0f);

	//fire 
	fireTimer -= dt;
	if (fireTimer <= 0 && Core::Input::IsPressed(VK_SPACE))
	{
		fireTimer = fireRate;

		/*{
		nc::Transform t = children[1]->transform;
		t.scale = 1.0f;

		scene->engine->Get<nc::AudioSystem>()->PlayAudio("player_fire");
	
		std::unique_ptr<Projectile> projectile = std::make_unique<Projectile>(t, scene->engine->Get<nc::ResourceSystem>()->Get<nc::Shape>("Player.txt"), 600.0f);
		projectile->tag = "Player";
		scene->AddActor(std::move(projectile));

		}

		{
			nc::Transform t = children[2]->transform;
			t.scale = 1.0f;

			scene->engine->Get<nc::AudioSystem>()->PlayAudio("player_fire");

			std::unique_ptr<Projectile> projectile = std::make_unique<Projectile>(t, scene->engine->Get<nc::ResourceSystem>()->Get<nc::Shape>("Player.txt"), 600.0f);
			projectile->tag = "Player";
			scene->AddActor(std::move(projectile));
		}*/
	
		nc::Transform t = children[0]->transform;
		t.scale = 1.0f;

		scene->engine->Get<nc::AudioSystem>()->PlayAudio("player_fire");

		std::unique_ptr<Projectile> projectile = std::make_unique<Projectile>(t, scene->engine->Get<nc::ResourceSystem>()->Get<nc::Shape>("Player_Laser.txt"), 1000.0f);
		projectile->tag = "Player";
		scene->AddActor(std::move(projectile));
	}

	std::vector<nc::Color> colors = { nc::Color::white, nc::Color::blue, nc::Color::red, nc::Color::green };
	scene->engine->Get<nc::ParticleSystem>()->Create(children[0]->transform.position, 1, 2, colors, 50, children[0]->transform.rotation, nc::DegToRad(30));
	
	Actor::Update(dt);
}

void Player::OnCollision(Actor* actor)
{
	if (dynamic_cast<Enemy*>(actor) || (dynamic_cast<Projectile*>(actor) && actor->tag == "Enemy"))
	{
		scene->engine->Get<nc::ParticleSystem>()->Create(transform.position, 200, 1, nc::Color::orange, 50);
		scene->engine->Get<nc::AudioSystem>()->PlayAudio("explosion");

		nc::Event event;
		event.name = "PlayerDead";
		scene->engine->Get<nc::EventSystem>()->Notify(event);

	}

	if (dynamic_cast<Projectile*>(actor) && actor->tag == "Enemy")
	{
		actor->destroy = true;
	}
}

