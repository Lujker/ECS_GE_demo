#pragma once
#include <string>
#include "gl_Include.h"

class Engine
{
public:
	Engine();
	~Engine();
	bool Init(const std::string& init_path);
	static void Destroy() noexcept;
	int DispatchEvents();
	void SwapBuffers();
private:
	GLFWwindow* m_window;
};