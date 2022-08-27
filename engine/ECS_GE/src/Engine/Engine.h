#pragma once
#include <string>
#include "ServiceLocator.h"
#include "gl_Include.h"

class Engine
{
public:
	Engine();
	~Engine();
	static ServiceLocator* getServiceLocator();
	bool Init(const std::string& init_path, ServiceLocator* service_locator = nullptr);
	bool IsInit() { return initial; }
	void Destroy() noexcept;
	int DispatchEvents();
	void SwapBuffers();
	Engine(const Engine&) = delete;
	Engine& operator=(const Engine&) = delete;
	Engine(Engine&&) noexcept = default;
	Engine& operator=(Engine&&) noexcept = default;
private:
	static ServiceLocator* g_service_locator;
	GLFWwindow* m_window;
	bool initial = false;
};
