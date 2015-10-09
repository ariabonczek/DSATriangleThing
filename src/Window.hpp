#ifndef WINDOW_HPP
#define WINDOW_HPP

#include "Config.hpp"

NS_BEGIN

/// <summary>
/// Handles any window-specific operations
/// </summary>
class Window
{
public:
	Window();
	~Window();

	/// <summary>
	/// Sets up the window
	/// </summary>
	void Initialize(uint width, uint height, std::string title);

	/// <summary>
	/// Destroys the window
	/// </summary>
	void Shutdown();

	/// <summary>
	/// Returns whether or not the window should close
	/// </summary>
	int ShouldClose();

	/// <summary>
	/// Clears the color buffer bit
	/// </summary>
	void Clear();

	/// <summary>
	/// Clears the color buffer bit with the given color
	/// </summary>
	void Clear(float x, float y, float z);

	/// <summary>
	/// Swaps the buffers and displays the back buffer
	/// </summary>
	void Display();

#if LUMINA_DEBUG
	void UpdateFrameRate();
	void PrintFrameRate();
	uint GetFrameRate()const;
#endif

private:
	GLFWwindow* window;
#if LUMINA_DEBUG
	float elapsedTime1;
	float elapsedTime5;
	uint frameCount;
	uint frameRate;
#endif
};

NS_END

#endif