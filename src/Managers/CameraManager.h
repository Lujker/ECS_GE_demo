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
	static void Init(const FRect& rect, double near, double far);
	static void UseShader(const std::shared_ptr<RenderEngine::ShaderProgram>& shader);
	static void Move(int x, int y);
	static void Resize(int width, int height);
	static FRect getProjRect();
	static const glm::mat4 getOrthMatrix();
	static void glfwWindowsSizeCallback(GLFWwindow* pWindow, int width, int height);
	static void glfwWindowsResize(GLFWwindow* pWindow, int width, int height);

private:
	static FRect projMatrix;
	static FRect activeWindowSize;
	static double near;
	static double far;
	CameraManager() = default;
	~CameraManager() = default;
	CameraManager(const CameraManager&) = delete;
	CameraManager(CameraManager&&) = delete;
	CameraManager& operator=(const CameraManager&) = delete;
	CameraManager& operator=(CameraManager&&) = delete;
};
