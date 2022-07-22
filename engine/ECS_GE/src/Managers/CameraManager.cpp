#include "CameraManager.h"
#include "ShaderProgram.h"
#include "UpdateSystem.h"

using namespace std;

CameraListener::CameraListener()
{
	CAMERA.AddListener(this);
}

CameraListener::~CameraListener()
{
	CAMERA.RemoveListener(this);
}

CameraManager& CameraManager::Instanse()
{
	static CameraManager camera;
	return camera;
}

void CameraManager::glfwWindowsSizeCallback(GLFWwindow* pWindow, int width, int height)
{
	CAMERA.setActiveWindowSize(width, height);
    RENDER.setViewport(CAMERA.getActiveWindowRect().mWidth, CAMERA.getActiveWindowRect().mHeight);
	for(const auto& it : CAMERA.m_listeners)
	{
		if (it)
			it->WindowResizeCallback(CAMERA.getActiveWindowRect());
	}
}

void CameraManager::glfwWindowsResize(GLFWwindow* pWindow, int width, int height)
{
	CAMERA.setActiveWindowSize(width, height);
	glfwSetWindowSize(pWindow, CAMERA.getActiveWindowRect().mWidth, CAMERA.getActiveWindowRect().mHeight);
	for (const auto& it : CAMERA.m_listeners)
	{
		if (it)
			it->WindowResizeCallback(CAMERA.getActiveWindowRect());
	}
}

void CameraManager::glfwWindowFocusCallback(GLFWwindow* window, int focused)
{
	CAMERA.fockused = focused;
	for (const auto& it : CAMERA.m_listeners)
	{
		if (it)
			it->WindowFocusCallback(CAMERA.fockused);
	}
}

void CameraManager::Init(const FRect& rect, const FRect& worldRect, double d_near, double d_far)
{
	if ((worldRect.mWidth - worldRect.mX) >= activeWindowSize.mWidth && (worldRect.mHeight - worldRect.mY) >= activeWindowSize.mHeight)
		this->worldRect = worldRect;
	else
		this->worldRect = activeWindowSize;
	projMatrix = rect;
	m_near = d_near;
	m_far = d_far;
}

void CameraManager::SetWorldRect(const FRect& rect)
{
	if ((rect.mWidth - rect.mX) >= activeWindowSize.mWidth && (rect.mHeight - rect.mY) >= activeWindowSize.mHeight)
		this->worldRect = rect;
	else
		this->worldRect = activeWindowSize;
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

void CameraManager::Move(double x, double y)
{
	auto next_rect = projMatrix;
	next_rect.mX += x;
	next_rect.mY += y;
	next_rect.mWidth += x;
	next_rect.mHeight += y;

	if (next_rect.mX < worldRect.mX || (next_rect.mWidth + next_rect.mX > worldRect.mWidth + worldRect.mX))
	{
		next_rect.mX -= x;
		next_rect.mWidth -= x;
	}
	if (next_rect.mY < worldRect.mY || (next_rect.mHeight + next_rect.mY > worldRect.mHeight + worldRect.mY))
	{
		next_rect.mY -= y;
		next_rect.mHeight -= y;
	}
	projMatrix = next_rect;
	for (const auto& it : CAMERA.m_listeners)
	{
		if (it)
			it->CameraMoveCallback(projMatrix);
	}
}

void CameraManager::SetCenterPoint(FPoint point)
{
	auto next_rect = projMatrix;
	const auto width = projMatrix.mWidth - projMatrix.mX;
	const auto height = projMatrix.mHeight - projMatrix.mY;
	next_rect.mX = (point.mX - width/2);
	next_rect.mY = (point.mY - height/2);
	next_rect.mWidth = (point.mX + width / 2);
	next_rect.mHeight = (point.mY + height / 2);

	if (next_rect.mX == projMatrix.mX && next_rect.mY == projMatrix.mY)
		return;

	if (next_rect.mX < worldRect.mX || (next_rect.mWidth + next_rect.mX > worldRect.mWidth + worldRect.mX))
	{
		next_rect.mX = projMatrix.mX;
		next_rect.mWidth = projMatrix.mWidth;
	}
	if (next_rect.mY < worldRect.mY || (next_rect.mHeight + next_rect.mY > worldRect.mHeight + worldRect.mY))
	{
		next_rect.mY = projMatrix.mY;
		next_rect.mHeight = projMatrix.mHeight;
	}
	projMatrix = next_rect;
	for (const auto& it : CAMERA.m_listeners)
	{
		if (it)
			it->CameraMoveCallback(projMatrix);
	}
}

void CameraManager::Resize(int width, int height)
{
	projMatrix.mWidth = width;
	projMatrix.mHeight += height;
}

bool CameraManager::AddListener(CameraListener* listener)
{
	auto it = std::find(m_listeners.begin(), m_listeners.end(), listener);
	if (it == m_listeners.end())
	{
		m_listeners.push_back(listener);
		return true;
	}
	return false;
}

bool CameraManager::RemoveListener(CameraListener* listener)
{
	auto it = std::find(m_listeners.begin(), m_listeners.end(), listener);
	if (it != m_listeners.end())
	{
		m_listeners.erase(std::remove(m_listeners.begin(), m_listeners.end(), *it), m_listeners.end());
		return true;
	}
	return false;
}

void CameraManager::ClearListeners()
{
	m_listeners.clear();
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

FRect CameraManager::getWorldRect()
{
	return worldRect;
}

bool CameraManager::windowOnFocus()
{
	return fockused;
}

void CameraManager::setActiveWindowSize(int width, int height)
{
	activeWindowSize = { 0,0, static_cast<float>(width), static_cast<float>(height) };
}
