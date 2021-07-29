#include "Game.h"
#include "Actors/Player.h"
#include "Actors/Enemy.h"
#include "Actors/Asteroid.h"
#include "Actors/Projectile.h"

void Game::Initialize()
{
	engine = std::make_unique<nc::Engine>(); // new engine
	engine->Startup();
	scene = std::make_unique<nc::Scene>(); // new scene
	scene->engine = engine.get();

	engine->Get<nc::AudioSystem>()->AddAudio("explosion", "explosion.wav");
	engine->Get<nc::AudioSystem>()->AddAudio("player_fire", "player_fire.wav");
	engine->Get<nc::AudioSystem>()->AddAudio("enemy_fire", "Laser_Shoot.wav");


	engine->Get<nc::EventSystem>()->Subscribe("AddPoints", std::bind(&Game::OnAddPoints, this, std::placeholders::_1));
	engine->Get<nc::EventSystem>()->Subscribe("PlayerDead", std::bind(&Game::OnPlayerDead, this, std::placeholders::_1));
}

void Game::Shutdown()
{
	scene->RemoveAllActors();
	engine->Shutdown();

}

void Game::Update(float dt)
{
	stateTimer += dt;

	switch (state)
	{
	case Game::eState::Title:
		if (Core::Input::IsPressed(VK_SPACE))
		{
			state = eState::StartGame;
		}
		break;
	case Game::eState::StartGame:
		score = 0;
		health = 200;
		lives = 3;
		spawnRate = 1;
		state = eState::StartLevel;
		break;
	case Game::eState::StartLevel:
	{	
		gameTimer += dt;
		scene->AddActor(std::make_unique<Player>(nc::Transform{ { 400, 200}, 0 , 5 }, engine->Get<nc::ResourceSystem>()->Get<nc::Shape>("Player.txt"), 300));
		scene->AddActor(std::make_unique<Asteroid>(nc::Transform{ {nc::RandomRange(0.0f, 800.0f), nc::RandomRange(0.0f, 600.0f), }, nc::RandomRange(0, nc::TwoPi), 3.0f }, engine->Get<nc::ResourceSystem>()->Get<nc::Shape>("Asteroid.txt"), 100));
		for (size_t i = 0; i < 3; i++)
		{
			scene->AddActor(std::make_unique<Asteroid>(nc::Transform{ {nc::RandomRange(0.0f, 800.0f), nc::RandomRange(0.0f, 600.0f), }, nc::RandomRange(0, nc::TwoPi), 3.0f }, engine->Get<nc::ResourceSystem>()->Get<nc::Shape>("Asteroid.txt"), 100));
			scene->AddActor(std::make_unique<Enemy>(nc::Transform{ {nc::RandomRange(0.0f, 800.0f), nc::RandomRange(0.0f, 600.0f), }, nc::RandomRange(0, nc::TwoPi), 3.5f }, engine->Get<nc::ResourceSystem>()->Get<nc::Shape>("Enemy.txt"), 200));
		}

		spawnTimer = 3.0f;
		state = eState::Game;
	}
		break;
	case Game::eState::Game:
		spawnTimer -= dt;
		if (spawnTimer <= 0)
		{
			spawnTimer = nc::RandomRange(1.0f, 3.0f);

			for (size_t i = 0; i < spawnRate; i++)
			{
				scene->AddActor(std::make_unique<Asteroid>(nc::Transform{ {nc::RandomRange(0.0f, 800.0f), nc::RandomRange(0.0f, 600.0f), }, nc::RandomRange(0, nc::TwoPi), 3.0f }, engine->Get<nc::ResourceSystem>()->Get<nc::Shape>("Asteroid.txt"), 100));
				scene->AddActor(std::make_unique<Enemy>(nc::Transform{ {nc::RandomRange(0.0f, 800.0f), nc::RandomRange(0.0f, 600.0f) }, nc::RandomRange(0, nc::TwoPi), 4 }, engine->Get<nc::ResourceSystem>()->Get<nc::Shape>("Enemy.txt"), 200));

			}

		}

		if (scene->GetActors<Enemy>().size() == 0)
		{
			state = Game::eState::GameOver;
		}
		break;
	case Game::eState::GameOver:
		if (Core::Input::IsPressed(VK_SPACE))
		{
			state = eState::StartGame;
		}
		break;
	default:
		break;
	}

	engine->Update(dt);
	scene->Update(dt);

}

void Game::Draw(Core::Graphics& graphics)
{
	switch (state)
	{
	case Game::eState::Title:
		//graphics.SetColor(nc::Color::blue);
		graphics.DrawString(380, 300 + std::sin(stateTimer + 10.0f) * 20.0f, "Future AAA");
		graphics.DrawString(340, 360, "Press Space to Start");
		break;
	case Game::eState::StartGame:
		break;
	case Game::eState::StartLevel:
		break;
	case Game::eState::Game:
		break;
	case Game::eState::GameOver:
		if (scene->GetActors<Player>().size() == 0)
		{
			float time = gameTimer * 100;
			scene->RemoveAllActors();
			graphics.DrawString(350, 360, "GAME OVER");
			graphics.DrawString(300, 390, "Press Space to Play Again");
			graphics.DrawString(305, 420, "Time Survived");
			graphics.DrawString(410, 420, std::to_string(time).c_str());
		}
		else
		{ 
			float time = gameTimer * 100;
			graphics.DrawString(360, 360, "YOU WIN");
			graphics.DrawString(300, 390, "Press Space to Play Again");
			graphics.DrawString(300, 420, "Time Survived");
			graphics.DrawString(400, 420, std::to_string(time).c_str());
		}
		break;
	default:
		break;
	}

	//graphics.SetColor(nc::Color::white);
	graphics.DrawString(10, 20, "Score:");
	graphics.DrawString(60, 20, std::to_string(score).c_str());

	graphics.DrawString(715, 30, "Health:");
	graphics.DrawString(770, 30, std::to_string(health).c_str());
	
	graphics.DrawString(620, 30, "Lives:");
	graphics.DrawString(670, 30, std::to_string(lives).c_str());

	scene->Draw(graphics);
	engine->Draw(graphics);
}

void Game::OnAddPoints(const nc::Event& event)
{
	score += std::get<int>(event.data);
}

void Game::OnPlayerDead(const nc::Event& event)
{
	health -= 20;
	
	if (health <= 0 && lives <= 0)
	{
		scene->GetActor<Player>()->destroy = true;
		state = eState::GameOver;
		
	}
	else if(health <= 0)
	{
		lives--;
		scene->GetActor<Player>()->destroy = true;
		health = 100;
		scene->AddActor(std::make_unique<Player>(nc::Transform{ { 400, 200}, 0 , 5 }, engine->Get<nc::ResourceSystem>()->Get<nc::Shape>("Player.txt"), 300));
	}
}
