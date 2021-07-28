#include "Game.h"
#include "Actors/Player.h"
#include "Actors/Enemy.h"
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
		lives = 3;
		state = eState::StartLevel;
		break;
	case Game::eState::StartLevel:
	{
		scene->AddActor(std::make_unique<Player>(nc::Transform{ { 400, 100}, 0 , 3 }, engine->Get<nc::ResourceSystem>()->Get<nc::Shape>("Player.txt"), 300));
		for (size_t i = 0; i < 3; i++)
		{
			scene->AddActor(std::make_unique<Enemy>(nc::Transform{ {nc::RandomRange(0.0f, 800.0f), nc::RandomRange(0.0f, 600.0f) }, nc::RandomRange(0, nc::TwoPi), 2 }, engine->Get<nc::ResourceSystem>()->Get<nc::Shape>("Enemy.txt"), 200));
		}

		state = eState::Game;
	}
		break;
	case Game::eState::Game:
		if (scene->GetActors<Enemy>().size() == 0)
		{
			state = Game::eState::GameOver;
		}
		break;
	case Game::eState::GameOver:
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
		graphics.DrawString(340, 360, "Press alt + f4 to start");
		break;
	case Game::eState::StartGame:
		break;
	case Game::eState::StartLevel:
		break;
	case Game::eState::Game:
		break;
	case Game::eState::GameOver:
		graphics.DrawString(340, 360, "GAME OVER");
		break;
	default:
		break;
	}

	//graphics.SetColor(nc::Color::white);
	graphics.DrawString(30, 20, std::to_string(score).c_str());
	graphics.DrawString(770, 30, std::to_string(lives).c_str());

	scene->Draw(graphics);
	engine->Draw(graphics);
}

void Game::OnAddPoints(const nc::Event& event)
{
	score += std::get<int>(event.data);
}

void Game::OnPlayerDead(const nc::Event& event)
{
	lives--;
	std::cout << std::get<std::string>(event.data) << std::endl;
	state = eState::GameOver;
}
