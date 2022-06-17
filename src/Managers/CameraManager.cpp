#include "CameraManager.h"
#include "ShaderProgram.h"
#include "UpdateSystem.h"

using namespace std;
FRect CameraManager::projMatrix;
FRect CameraManager::activeWindowSize;
double CameraManager::m_near = 0.;
double CameraManager::m_far = 0.;
bool CameraManager::fockused = true;

CameraManager& CameraManager::Instanse()
{
	static CameraManager camera;
	return camera;
}

void CameraManager::glfwWindowsSizeCallback(GLFWwindow* pWindow, int width, int height)
{
	setActiveWindowSize(width, height);	
    RENDER.setViewport(getActiveWindowRect().mWidth, getActiveWindowRect().mHeight);
}

void CameraManager::glfwWindowsResize(GLFWwindow* pWindow, int width, int height)
{
	setActiveWindowSize(width, height);
	glfwSetWindowSize(pWindow, getActiveWindowRect().mWidth, getActiveWindowRect().mHeight);
}

void CameraManager::glfwWindowFocusCallback(GLFWwindow* window, int focused)
{
	CameraManager::fockused = focused;
}

void CameraManager::Init(const FRect& rect, double d_near, double d_far)
{
	CameraManager::projMatrix = rect;
	CameraManager::m_near = d_near;
	CameraManager::m_far = d_far;
}

void CameraManager::ReleaseShader()
{
	m_shader.reset();
}

void CameraManager::SetShader(const std::shared_ptr<RenderEngine::ShaderProgram>& shader)
{
	if (!shader || m_shader == shader)
		return;
	m_shader = shader;
}

void CameraManager::Update()
{
	if (!m_shader)
		return;
	const glm::mat4 projectionMatrix = glm::ortho(projMatrix.mX, projMatrix.mWidth, projMatrix.mY, projMatrix.mHeight, m_near, m_far);
	m_shader->use();
	m_shader->setMatrix4("projectionMatrix", projectionMatrix);
}

void CameraManager::UseShader(const std::shared_ptr<RenderEngine::ShaderProgram>& shader)
{
	if(!shader)
		return;
	const glm::mat4 projectionMatrix = glm::ortho(projMatrix.mX, projMatrix.mWidth, projMatrix.mY, projMatrix.mHeight, m_near, m_far);
	shader->use();
	shader->setMatrix4("projectionMatrix", projectionMatrix);
}

void CameraManager::Move(int x, int y)
{
	projMatrix.mX += x;
	projMatrix.mY += y;
}

void CameraManager::Resize(int width, int height)
{
	projMatrix.mWidth = width;
	projMatrix.mHeight += height;
}

FRect CameraManager::getProjRect()
{
	return projMatrix;
}

const glm::mat4 CameraManager::getOrthMatrix()
{
	return glm::ortho(projMatrix.mX, projMatrix.mWidth, projMatrix.mY, projMatrix.mHeight, m_near, m_far);
}

FRect CameraManager::getActiveWindowRect()
{
	return activeWindowSize;
}

bool CameraManager::windowOnFocus()
{
	return fockused;
}

void CameraManager::setActiveWindowSize(int width, int height)
{
	activeWindowSize = { 0,0, static_cast<float>(width), static_cast<float>(height) };
}
