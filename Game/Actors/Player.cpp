#include "Player.h"
#include "Enemy.h"
#include "Projectile.h"
#include "Engine.h"
#include <memory>

Player::Player(const nc::Transform& transform, std::shared_ptr<nc::Shape> shape, float speed) : nc::Actor{ transform, shape }, speed{ speed }
{
	std::unique_ptr locator = std::make_unique<Actor>();
	locator->transform.localPosition = nc::Vector2{ 8, 0 };
	AddChild(std::move(locator));

	locator = std::make_unique<Actor>();
	locator->transform.localPosition = nc::Vector2{ 0, 5 };
	AddChild(std::move(locator));
	
	/*locator = std::make_unique<Actor>();
	locator->transform.localPosition = nc::Vector2{ 0, -5 };
	AddChild(std::move(locator));*/
}

void Player::Update(float dt)
{

	//movement
	float thrust = 0;
	if (Core::Input::IsPressed('A')) transform.rotation += -5 * dt;
	if (Core::Input::IsPressed('D')) transform.rotation += 5 * dt;
	if (Core::Input::IsPressed('W')) thrust = speed;

	transform.position += nc::Vector2::Rotate(nc::Vector2::right, transform.rotation) * thrust * dt;
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
		t.scale = 0.5f;

		scene->engine->Get<nc::AudioSystem>()->PlayAudio("player_fire");

		std::unique_ptr<Projectile> projectile = std::make_unique<Projectile>(t, scene->engine->Get<nc::ResourceSystem>()->Get<nc::Shape>("Player.txt"), 600.0f);
		projectile->tag = "Player";
		scene->AddActor(std::move(projectile));
	}

	scene->engine->Get<nc::ParticleSystem>()->Create(transform.position, 1, 2, nc::Color::yellow, 50);
	
	Actor::Update(dt);
}

void Player::OnCollision(Actor* actor)
{
	return;

	if (dynamic_cast<Enemy*>(actor))
	{
		destroy = true;
		scene->engine->Get<nc::ParticleSystem>()->Create(transform.position, 200, 1, nc::Color::orange, 50);
		scene->engine->Get<nc::AudioSystem>()->AddAudio("explosion", "explosion.wav");

		nc::Event event;
		event.name = "PlayerDead";
		event.data = std::string("yes im dead");
		scene->engine->Get<nc::EventSystem>()->Notify(event);

	}
}

