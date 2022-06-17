#pragma once
#include <memory>
#include <glm/ext/matrix_float4x4.hpp>
#include "Rect.h"
#include "Systems/RenderSystem.h"
#define CAMERA CameraManager::Instanse()

namespace RenderEngine
{
	class ShaderProgram;
}

class CameraManager
{
public:
	static CameraManager& Instanse();
	static void Init(const FRect& rect, double near = -100., double far = 100.);
	void ReleaseShader();
	void SetShader(const std::shared_ptr<RenderEngine::ShaderProgram>& shader);
	void Update();
	static void UseShader(const std::shared_ptr<RenderEngine::ShaderProgram>& shader);
	static void Move(int x, int y);
	static void Resize(int width, int height);
	static FRect getProjRect();
	static const glm::mat4 getOrthMatrix();
	static FRect getActiveWindowRect();
	static void setActiveWindowSize(int width, int height);
	static void glfwWindowsSizeCallback(GLFWwindow* pWindow, int width, int height);
	static void glfwWindowsResize(GLFWwindow* pWindow, int width, int height);
private:
	std::shared_ptr<RenderEngine::ShaderProgram> m_shader;
	static FRect projMatrix;
	static FRect activeWindowSize;
	static double near;
	static double far;
	static Point mainWindowSize;
	CameraManager() = default;
	~CameraManager() = default;
	CameraManager(const CameraManager&) = delete;
	CameraManager(CameraManager&&) = delete;
	CameraManager& operator=(const CameraManager&) = delete;
	CameraManager& operator=(CameraManager&&) = delete;
};
