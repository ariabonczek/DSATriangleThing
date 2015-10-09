#ifndef INPUT_HPP
#define INPUT_HPP

#include "../Config.hpp"

NS_BEGIN

class Input
{
public:
	~Input();

	static void Initialize(GLFWwindow* _window);

	static bool GetKey(int key);
	static bool GetKeyDown(int key);
	static bool GetKeyUp(int key);

	static Vector2 GetMousePosition();
private:
	Input();

	static GLFWwindow* window;
	static char keyLast[GLFW_KEY_LAST + 1];
};

NS_END

#endif