#include "Game.h"
#include <variant>

Game game;

bool Update(float dt)
{
	bool quit = Core::Input::IsPressed(Core::Input::KEY_ESCAPE);
	game.Update(dt);

	/*deltaTime = dt;
	gameTime += dt;

	timer += dt *5;*/

	/*int x, y;
	Core::Input::GetMousePos(x, y);
	psPosition.x = static_cast<float>(x);
	psPosition.y = static_cast<float>(y);

	if (Core::Input::IsPressed(Core::Input::BUTTON_LEFT))
	{
		std::vector<nc::Color> colors = { nc::Color::white, nc::Color::red, nc::Color::blue, nc::Color::yellow };
		engine.Get<nc::ParticleSystem>()->Create(psPosition, 150, 2, colors[nc::RandomRangeInt(0, colors.size())], 150);

		engine.Get<nc::AudioSystem>()->PlayAudio("explosion");
	}*/

	//scene.GetActor<Player>()->shape->color = nc::Color{nc::RandomRange(0,1), nc::RandomRange(0,1), nc::RandomRange(0,1)};
	//scene.GetActor<Enemy>()->shape->color = nc::Color{nc::RandomRange(0,1), nc::RandomRange(0,1), nc::RandomRange(0,1)};

	return quit;
}

void Draw(Core::Graphics& graphics)
{
	game.Draw(graphics);
}

int inc(int i) { return ++i; }
int dec(int i) { return --i; }

union Data
{
	int i;
	char str[5];
	bool b;
};

int main()
{
	Data data{ 0 };
	data.b = true;
	data.i = 534;
	data.str[0] = '#';
	std::cout << data.i << std::endl;
	std::cout << data.b << std::endl;
	std::cout << data.str << std::endl;

	std::variant<int, std::string, float, bool> var;
	var = 20;
	var = true;

	//std::cout << std::get<int>(var) << std::endl;
	std::cout << std::get<bool>(var) << std::endl;
	std::cout << std::get<3>(var) << std::endl;

	/*int (*operation)(int);
	operation = &inc;

	std::cout << operation(5) << std::endl;*/
	//system("pause");

	char name[] = "CSC196";
	Core::Init(name, 800, 600, 60);
	Core::RegisterUpdateFn(Update);
	Core::RegisterDrawFn(Draw);

	game.Initialize();
	
	Core::GameLoop();
	Core::Shutdown();

	game.Shutdown();
}
