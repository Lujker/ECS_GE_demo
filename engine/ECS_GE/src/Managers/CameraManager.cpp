#include "CameraManager.h"
#include "Engine.h"
#include "ShaderProgram.h"
#include "UpdateSystem.h"

using namespace std;

CameraListener::CameraListener()
{
	CAMERA->AddListener(this);
}

CameraListener::~CameraListener()
{
	CAMERA->RemoveListener(this);
}

void CameraManager::glfwWindowsSizeCallback(GLFWwindow* pWindow, int width, int height)
{
	CAMERA->setActiveWindowSize(width, height);
    RENDER->setViewport(CAMERA->getActiveWindowRect().mWidth, CAMERA->getActiveWindowRect().mHeight);
	for(const auto& it : CAMERA->m_listeners)
	{
		if (it)
			it->WindowResizeCallback(CAMERA->getActiveWindowRect());
	}
}

void CameraManager::glfwWindowsResize(GLFWwindow* pWindow, int width, int height)
{
	CAMERA->setActiveWindowSize(width, height);
	glfwSetWindowSize(pWindow, CAMERA->getActiveWindowRect().mWidth, CAMERA->getActiveWindowRect().mHeight);
	for (const auto& it : CAMERA->m_listeners)
	{
		if (it)
			it->WindowResizeCallback(CAMERA->getActiveWindowRect());
	}
}

void CameraManager::glfwWindowFocusCallback(GLFWwindow* window, int focused)
{
	CAMERA->fockused = focused;
	for (const auto& it : CAMERA->m_listeners)
	{
		if (it)
			it->WindowFocusCallback(CAMERA->fockused);
	}
}

CameraManager::~CameraManager()
{
	INPUTS->ListenerRemove(this);
}

void CameraManager::Init(GLFWwindow* pWindow, const FRect& rect, const FRect& worldRect, double d_near, double d_far)
{
	if ((worldRect.mWidth - worldRect.mX) >= activeWindowSize.mWidth && (worldRect.mHeight - worldRect.mY) >= activeWindowSize.mHeight)
		this->worldRect = worldRect;
	else
		this->worldRect = activeWindowSize;
	projMatrix = rect;
	m_near = d_near;
	m_far = d_far;
	camPos.lastX = activeWindowSize.mWidth / 2.;
	camPos.lastY = activeWindowSize.mHeight / 2.;
	pMainWindow = pWindow;
	INPUTS->ListenerAdd(this);
}

void CameraManager::SetWorldRect(const FRect& rect)
{
	if ((rect.mWidth - rect.mX) >= activeWindowSize.mWidth && (rect.mHeight - rect.mY) >= activeWindowSize.mHeight)
		this->worldRect = rect;
	else
		this->worldRect = activeWindowSize;
}

void CameraManager::UpdateCameraPos(const FRect& rect)
{
	if ((rect.mWidth - rect.mX) >= activeWindowSize.mWidth && (rect.mHeight - rect.mY) >= activeWindowSize.mHeight)
		this->projMatrix = rect;
	else
		this->projMatrix = activeWindowSize;
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
	glm::mat4 proj;
	glm::mat4 view;

	if(camPos.isPerspective)
		proj = glm::perspective(camPos.fov, activeWindowSize.mWidth / activeWindowSize.mHeight, m_near, m_far );
	else
		proj = glm::ortho(projMatrix.mX, projMatrix.mWidth, projMatrix.mY, projMatrix.mHeight, -m_far, m_far);

	view = glm::lookAt(camPos.cameraPos, camPos.cameraPos + camPos.cameraFront, camPos.cameraUp);
	m_shader->use();
	m_shader->setMatrix4("projectionMatrix", proj);
	m_shader->setMatrix4("viewMatrix", view);
}

void CameraManager::UseShader(const std::shared_ptr<RenderEngine::ShaderProgram>& shader)
{
	if(!shader)
		return;
	glm::mat4 proj;
	glm::mat4 view;

	if (camPos.isPerspective)
		proj = glm::perspective(camPos.fov, activeWindowSize.mWidth / activeWindowSize.mHeight, m_near, m_far );
	else
		proj = glm::ortho(projMatrix.mX, projMatrix.mWidth, projMatrix.mY, projMatrix.mHeight, -m_far, m_far);

	view = glm::lookAt(camPos.cameraPos, camPos.cameraPos + camPos.cameraFront, camPos.cameraUp);
	shader->use();
	shader->setMatrix4("projectionMatrix", proj);
	shader->setMatrix4("viewMatrix", view);
}

void CameraManager::ChangeCursorInputMode(CursorInputMode mode)
{
	glfwSetInputMode(pMainWindow, GLFW_CURSOR, static_cast<int>(mode));
	camPos.cursorInputeMode = mode;
}

void CameraManager::SetPerspectiveProj()
{
	if (pMainWindow)
		ChangeCursorInputMode(CursorInputMode::eDisable);
	camPos.isPerspective = true;
}

void CameraManager::SetOrthProj()
{
	if (pMainWindow)
		ChangeCursorInputMode(CursorInputMode::eNormal);
	camPos.isPerspective = false;
}

void CameraManager::ClearCamPos()
{
	camPos.cameraPos = glm::vec3(0.0f, 0.0f, 1.0f);
	camPos.cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	camPos.cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

	camPos.lastX = 0; camPos.lastY = 0;
	camPos.yaw = -90.0f;
	camPos.pitch = 0.0f;
	camPos.fov = 45.f;
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
	for (const auto& it : m_listeners)
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
	for (const auto& it : m_listeners)
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

void CameraManager::UpdateMoveCamera(float delta_time)
{
	auto speed = camPos.cameraSpeed * delta_time;
	auto& keys = INPUTS->GetKeyMap();
	if (keys[GLFW_KEY_UP])
		camPos.cameraPos += speed * camPos.cameraFront;
	if (keys[GLFW_KEY_DOWN])
		camPos.cameraPos -= speed * camPos.cameraFront;
	if (keys[GLFW_KEY_LEFT])
		camPos.cameraPos -= glm::normalize(glm::cross(camPos.cameraFront, camPos.cameraUp)) * speed;
	if (keys[GLFW_KEY_RIGHT])
		camPos.cameraPos += glm::normalize(glm::cross(camPos.cameraFront, camPos.cameraUp)) * speed;
}

void CameraManager::KeyPress(const int& key)
{
	if (key >= 262 && key <= 265)
		camPos.cameraSpeed += 0.1f;
}

void CameraManager::KeyUnpress(const int& key)
{
	if (key >= 262 && key <= 265)
		camPos.cameraSpeed -= 0.1f;
	else if (key == GLFW_KEY_F1)
	{
		switch (camPos.cursorInputeMode)
		{
		case CursorInputMode::eNormal:
			ChangeCursorInputMode(CursorInputMode::eHiden);
			break;
		case CursorInputMode::eHiden:
			ChangeCursorInputMode(CursorInputMode::eDisable);
			break;
		default:
			ChangeCursorInputMode(CursorInputMode::eNormal);
			break;
		}
	}
}

void CameraManager::MouseMove(const FPoint& current_pos)
{
	if (!camPos.isPerspective || camPos.cursorInputeMode == CursorInputMode::eNormal)
		return;
	GLfloat xoffset = current_pos.mX - camPos.lastX;
	GLfloat yoffset = camPos.lastY - current_pos.mY; // Обратный порядок вычитания потому что оконные Y-координаты возрастают с верху вниз 
	camPos.lastX = current_pos.mX;
	camPos.lastY = current_pos.mY;
	xoffset *= camPos.sensitivity;
	yoffset *= camPos.sensitivity;

	camPos.yaw += xoffset;
	camPos.pitch -= yoffset;
	if (camPos.pitch > 89.0f)
		camPos.pitch = 89.0f;
	if (camPos.pitch < -89.0f)
		camPos.pitch = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(camPos.yaw)) * cos(glm::radians(camPos.pitch));
	front.y = sin(glm::radians(camPos.pitch));
	front.z = sin(glm::radians(camPos.yaw)) * cos(glm::radians(camPos.pitch));
	camPos.cameraFront = glm::normalize(front);
}

void CameraManager::MouseScroll(const float& fov)
{
	if (!camPos.isPerspective || camPos.cursorInputeMode == CursorInputMode::eNormal)
		return;
	if (camPos.fov >= 1.0f && camPos.fov <= 45.0f)
		camPos.fov -= fov * camPos.sensitivity;
	if (camPos.fov <= 1.0f)
		camPos.fov = 1.0f;
	if (camPos.fov >= 45.0f)
		camPos.fov = 45.0f;
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

CameraManager::CameraPosition CameraManager::getCameraPosition()
{
	return camPos;
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
