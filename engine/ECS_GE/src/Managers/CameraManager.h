#pragma once
#include <deque>
#include <memory>
#include <glm/ext/matrix_float4x4.hpp>
#include "Rect.h"
#include "Systems/RenderSystem.h"
#include "InputManager.h"

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

class CameraManager : public InputListener
{
public:

	enum class CursorInputMode
	{
		eNormal = GLFW_CURSOR_NORMAL,
		eHiden = GLFW_CURSOR_HIDDEN,
		eDisable = GLFW_CURSOR_DISABLED
	};

	struct CameraPosition
	{
		bool isPerspective = true;
		float cameraSpeed = 0.1f;
		glm::vec3 cameraPos = glm::vec3(200.0f, 200.0f, 300.0f);
		glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
		glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

		GLfloat lastX = 0, lastY = 0;
		float sensitivity = 0.05f;
		GLfloat yaw = -90.0f;
		GLfloat pitch = 0.0f;
		double fov = 45.f;
		CursorInputMode cursorInputeMode = CursorInputMode::eNormal;
	};

	friend class ServiceLocator;
	friend class ImGuiController;
	~CameraManager() override;
	void Init(GLFWwindow* pWindow, const FRect& rect, const FRect& worldRect = { 0,0,0,0}, double near = 0.1, double far = 1000.);
	void SetWorldRect(const FRect& rect);
	void UpdateCameraPos(const FRect& rect);
	void ReleaseShader();
	void SetShader(const std::shared_ptr<RenderEngine::ShaderProgram>& shader);
	void Update();
	void UseShader(const std::shared_ptr<RenderEngine::ShaderProgram>& shader);
	void ChangeCursorInputMode(CursorInputMode mode);
	void SetPerspectiveProj();
	void SetOrthProj();
	void ClearCamPos();
	void Move(double x, double y);
	void SetCenterPoint(FPoint point);
	void Resize(int width, int height);
	void UpdateMoveCamera(float delta_time);
	void KeyPress(const int& key) override;
	void KeyUnpress(const int& key) override;
	void MouseMove(const FPoint& current_pos) override;
	void MouseScroll(const float& fov) override;

	bool AddListener(CameraListener*);
	bool RemoveListener(CameraListener*);
	void ClearListeners();

	CameraPosition getCameraPosition();
	FRect getProjRect();
	FRect getActiveWindowRect();
	FRect getWorldRect();
	const glm::mat4 getOrthMatrix();
	bool windowOnFocus();
	[[nodiscard]] double	getNearLayer() const { return m_near; }
	[[nodiscard]] double	getFarLayer() const { return m_far; }
	[[nodiscard]] bool		isDrawDebugInfo() const { return drawDebugInfo; }
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
	double m_near = 0.1;
	double m_far = 10000.;
	Point mainWindowSize;
	bool fockused = true;
	bool drawDebugInfo = true;
	CameraPosition camPos;
	GLFWwindow* pMainWindow = nullptr;
	CameraManager() = default;
};
