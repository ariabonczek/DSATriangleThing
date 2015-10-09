#ifndef GAME_HPP
#define GAME_HPP

#include "Config.hpp"
#include "Window.hpp"
#include "TestScene.hpp"

NS_BEGIN

/// <summary>
/// Singleton class, represents the whole game
/// </summary>
class Game
{
public:
	~Game();

	/// <summary>
	/// Returns the game instance
	/// </summary>
	static Game* GetInstance();

	/// <summary>
	/// Runs the game
	/// </summary>
	int Run();
private:
	Game();
	static Game instance;
	Window window;
	TestScene scene;

	bool Initialize();
	bool Shutdown();
};

NS_END

#endif