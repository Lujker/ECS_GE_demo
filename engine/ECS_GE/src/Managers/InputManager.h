#pragma once

#include "gl_Include.h"
#include <deque>
#include <map>

#define INPUTS InputManager::Instanse()
#include "Point.h"

class InputListener
{
public:
	virtual ~InputListener() = 0;

	virtual void KeyPress(const int& key);
	virtual void KeyUnpress(const int& key);
	virtual void MousePress(const int& key);
	virtual void MouseUnpress(const int& key);
	virtual void MouseMove(const FPoint& current_pos);
};

class InputManager
{
public:
	static InputManager& Instanse();
	static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void ÑursorPositionCallback(GLFWwindow* window, double xpos, double ypos);
	static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
private:
	static std::map<int, bool> key_map;
	static FPoint last_cursor_pos;
	static std::deque<InputListener*> listeners;
	InputManager() = default;
	~InputManager() = default;
	InputManager(const InputManager&) = delete;
	InputManager(InputManager&&) = delete;
	InputManager& operator=(const InputManager&) = delete;
	InputManager& operator=(InputManager&&) = delete;
	
};