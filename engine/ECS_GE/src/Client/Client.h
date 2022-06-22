#pragma once
#include <string>
#include "gl_Include.h"

class Client
{
public:
	Client();
	~Client();
	bool Init(const std::string& init_path);
	static void Destroy() noexcept;
	void DeltaLoop();
private:
	GLFWwindow* m_window;
};
