#include "Game.hpp"

#include "Utility\Timer.hpp"

NS_BEGIN

Game Game::instance;

Game::~Game()
{

}

Game* Game::GetInstance()
{
	return &instance;
}

int Game::Run()
{
	Initialize();

	scene.LoadAssets();

	// Game Loop
	while (!window.ShouldClose())
	{
		Timer::StartFrame();
		window.Clear();

		scene.Update(Timer::GetFrameTime());
		scene.Draw();
		
		window.Display();
		Timer::StopFrame();
#if LUMINA_DEBUG
		window.UpdateFrameRate();
		window.PrintFrameRate();
#endif
	}

	scene.UnloadAssets();

	Shutdown();

	return 1;
}

Game::Game()
{

}

bool Game::Initialize()
{
	Timer::Initialize();

	if (!glfwInit())
		return false;

	window.Initialize(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE);

	glewExperimental = true;
	if (glewInit() != GLEW_OK)
		return false;

	return true;
}

bool Game::Shutdown()
{
	window.Shutdown();

	return true;
}



NS_END