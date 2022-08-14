#pragma once
#include <deque>
#include <memory>
#include <glm/ext/matrix_float4x4.hpp>
#include "Rect.h"
#include "Systems/RenderSystem.h"

namespace RenderEngine
{
	class ShaderProgram;
}

class CameraListener
{
public:
	CameraListener();
	virtual ~CameraListener();
	virtual void WindowResizeCallback(const FRect&) {}
	virtual void CameraMoveCallback(const FRect&) {}
	virtual void WindowFocusCallback(bool) {}
};

class CameraManager
{
public:
	friend class ServiceLocator;
	void Init(const FRect& rect, const FRect& worldRect = { 0,0,0,0}, double near = -100., double far = 100.);
	void SetWorldRect(const FRect& rect);
	void UpdateCameraPos(const FRect& rect);
	void ReleaseShader();
	void SetShader(const std::shared_ptr<RenderEngine::ShaderProgram>& shader);
	void Update();
	void UseShader(const std::shared_ptr<RenderEngine::ShaderProgram>& shader);
	void Move(double x, double y);
	void SetCenterPoint(FPoint point);
	void Resize(int width, int height);

	bool AddListener(CameraListener*);
	bool RemoveListener(CameraListener*);
	void ClearListeners();

	FRect getProjRect();
	FRect getActiveWindowRect();
	FRect getWorldRect();
	const glm::mat4 getOrthMatrix();
	bool windowOnFocus();
	[[nodiscard]] double getNearLayer() const { return m_near; }
	[[nodiscard]] double getFarLayer() const { return m_far; }
	[[nodiscard]] bool isDrawDebugInfo() const { return drawDebugInfo; }
	void setDrawDebugInfo(bool need_draw) { drawDebugInfo = need_draw; }
	void setActiveWindowSize(int width, int height);

	static void glfwWindowsSizeCallback(GLFWwindow* pWindow, int width, int height);
	static void glfwWindowsResize(GLFWwindow* pWindow, int width, int height);
	static void glfwWindowFocusCallback(GLFWwindow* window, int focused);

	CameraManager(const CameraManager&) = delete;
	CameraManager(CameraManager&&) = delete;
	CameraManager& operator=(const CameraManager&) = delete;
	CameraManager& operator=(CameraManager&&) = delete;
private:
	std::shared_ptr<RenderEngine::ShaderProgram> m_shader;
	std::deque<CameraListener*> m_listeners;
	FRect projMatrix;
	FRect activeWindowSize;
	FRect worldRect;
	double m_near = -100.;
	double m_far = 100.;
	Point mainWindowSize;
	bool fockused = true;
	bool drawDebugInfo = true;
	CameraManager() = default;
	~CameraManager() = default;
};
