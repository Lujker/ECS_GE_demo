#pragma once
#include <string>
#include "gl_Include.h"

class Engine
{
public:
	Engine();
	~Engine();
	bool Init(const std::string& init_path);
	bool IsInit() { return initial; }
	void Destroy() noexcept;
	int DispatchEvents();
	void SwapBuffers();
private:
	Engine(const Engine&) = delete;
	Engine& operator=(const Engine&) = delete;

	GLFWwindow* m_window;
	bool initial = false;
};