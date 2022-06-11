#include "CameraManager.h"
#include "ShaderProgram.h"

FRect CameraManager::projMatrix;
FRect CameraManager::activeWindowSize;
double CameraManager::near;
double CameraManager::far;

CameraManager& CameraManager::Instanse()
{
	static CameraManager camera;
	return camera;
}

void CameraManager::glfwWindowsSizeCallback(GLFWwindow* pWindow, int width, int height)
{
	activeWindowSize.mWidth = width;
	activeWindowSize.mHeight = height;
	
    RENDER.setViewport(activeWindowSize.mWidth, activeWindowSize.mHeight);
}

void CameraManager::glfwWindowsResize(GLFWwindow* pWindow, int width, int height)
{
	glfwSetWindowSize(pWindow, width, height);
}

void CameraManager::Init(const FRect& rect, double near, double far)
{
	CameraManager::projMatrix = rect;
	CameraManager::near = near;
	CameraManager::far = far;
}

void CameraManager::UseShader(const std::shared_ptr<RenderEngine::ShaderProgram>& shader)
{
	if(!shader)
		return;
	shader->use();
	const glm::mat4 projectionMatrix = glm::ortho(projMatrix.mX, projMatrix.mWidth, projMatrix.mY, projMatrix.mHeight, near, far);
	shader->use();
	shader->setMatrix4("projectionMatrix", projectionMatrix);
}

void CameraManager::Move(int x, int y)
{
	CameraManager::projMatrix.mX += x;
	CameraManager::projMatrix.mY += y;
}

void CameraManager::Resize(int width, int height)
{
	CameraManager::projMatrix.mWidth = width;
	CameraManager::projMatrix.mHeight += height;
}

FRect CameraManager::getProjRect()
{
	return CameraManager::projMatrix;
}

const glm::mat4 CameraManager::getOrthMatrix()
{
	return glm::ortho(projMatrix.mX, projMatrix.mWidth, projMatrix.mY, projMatrix.mHeight, near, far);
}
