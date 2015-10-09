#include "Input.hpp"

NS_BEGIN

GLFWwindow* Input::window;
char Input::keyLast[GLFW_KEY_LAST + 1];

Input::~Input()
{

}

void Input::Initialize(GLFWwindow* _window)
{
	window = _window;
}

bool Input::GetKey(int key)
{	
	return glfwGetKey(window, key) ? true : false;
}

bool Input::GetKeyDown(int key)
{
	bool flag = true;
	if (glfwGetKey(window, key))
	{
		keyLast[key] ? (flag = false) : (keyLast[key] = true);
	}
	else
	{
		keyLast[key] = false;
		flag = false;
	}
	return flag;
}

bool Input::GetKeyUp(int key)
{
	return !glfwGetKey(window, key);
}

Vector2 Input::GetMousePosition()
{
	double x, y;
	glfwGetCursorPos(window, &x, &y);
	return Vector2((float)x, (float)y);
}

Input::Input()
{

}

NS_END