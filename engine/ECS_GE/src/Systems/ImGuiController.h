#pragma once
#include "gl_Include.h"

class ImGuiController
{
public:
	friend class ServiceLocator;

	virtual void DrawController();

	ImGuiController(const ImGuiController&) = delete;
	ImGuiController(ImGuiController&&) = delete;
	ImGuiController& operator=(const ImGuiController&) = delete;
	ImGuiController& operator=(ImGuiController&&) = delete;

protected:
	virtual void DrawMetricInfo();
	virtual void init(GLFWwindow* main_window);
	void destroy();
	ImGuiController() = default;
	virtual ~ImGuiController();

protected:
	bool isInit = false;
};