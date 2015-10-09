#include "Window.hpp"
#include "Utility\Input.hpp"
#include "Utility\Timer.hpp"

NS_BEGIN

Window::Window()
{

}

Window::~Window()
{

}

void Window::Initialize(uint width, uint height, std::string title)
{
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	// Enable multisampling
	glfwWindowHint(GLFW_SAMPLES, MSAA_SAMPLES);

	window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
	glfwMakeContextCurrent(window);

	glfwSwapInterval(1);

	// Depth test
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	// Back-face culling
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CW);

	// MSAA
	glEnable(GL_MULTISAMPLE);

	glClearColor(0.392f, 0.584f, 0.929f, 1.0f);
	//glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	Input::Initialize(window);
}

void Window::Clear()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_DEPTH_BUFFER_BIT);
}

void Window::Clear(float x, float y, float z)
{
	glClearColor(x, y, z, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_DEPTH_BUFFER_BIT);
}

void Window::Shutdown()
{
	glfwDestroyWindow(window);
}

int Window::ShouldClose()
{
	return glfwWindowShouldClose(window);
}

void Window::Display()
{
	glfwSwapBuffers(window);
	glfwPollEvents();
}

#if LUMINA_DEBUG
void Window::UpdateFrameRate()
{
	elapsedTime1 += Timer::GetFrameTime();

	if (elapsedTime1 > 1.0f)
	{
		elapsedTime1 -= 1.0f;
		frameRate = frameCount;
		frameCount = 0;
	}

	frameCount++;
}

void Window::PrintFrameRate()
{
	elapsedTime5 += Timer::GetFrameTime();

	if (elapsedTime5 > 5.0f)
	{
		elapsedTime5 -= 5.0f;
		std::cout << "FPS: " << frameRate << std::endl;
	}
}

uint Window::GetFrameRate()const
{
	return frameRate;
}
#endif
NS_END