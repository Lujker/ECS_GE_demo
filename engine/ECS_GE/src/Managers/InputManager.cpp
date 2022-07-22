#include "InputManager.h"

#include "CameraManager.h"

std::map<int, bool> InputManager::key_map;
FPoint InputManager::last_cursor_pos;
std::vector<InputListener*> InputManager::listeners;

InputManager& InputManager::Instanse()
{
	static InputManager input_manager;
	return input_manager;
}

void InputManager::ListenerAdd(InputListener* listener)
{
	if (listener != nullptr)
		listeners.push_back(listener);
}

void InputManager::ListenerRemove(InputListener* listener)
{
	const auto it = std::find(listeners.begin(), listeners.end(), listener);
	if (it != listeners.end())
	{
		listeners.erase(std::remove(listeners.begin(), listeners.end(), *it), listeners.end());
	}
}

void InputManager::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		key_map[key] = true;
		for (const auto& it : listeners)
		{
			it->KeyPress(key);
		}
	}
	if (action == GLFW_RELEASE)
	{
		key_map[key] = false;
		for (const auto& it : listeners)
		{
			it->KeyUnpress(key);
		}
	}
}

void InputManager::ÑursorPositionCallback(GLFWwindow* window, double xpos, double ypos)
{
	last_cursor_pos = { xpos,CAMERA.getActiveWindowRect().mHeight - ypos };
	for(const auto& it : listeners)
	{
		it->MouseMove(last_cursor_pos);
	}
}

void InputManager::MouseButtonCallback(GLFWwindow* window, int key, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		key_map[key] = true;
		for (const auto& it : listeners)
		{
			it->MousePress(key);
		}
	}
	if (action == GLFW_RELEASE)
	{
		key_map[key] = false;
		for (const auto& it : listeners)
		{
			it->MouseUnpress(key);
		}
	}
}

FPoint InputManager::GetLastCursorPos()
{
	return last_cursor_pos;
}

InputListener::~InputListener()
{}

void InputListener::KeyPress(const int& key)
{}

void InputListener::KeyUnpress(const int& key)
{}

void InputListener::MousePress(const int& key)
{}

void InputListener::MouseUnpress(const int& key)
{}

void InputListener::MouseMove(const FPoint& current_pos)
{}
