#include "Engine.h"

#include "CameraManager.h"
#include "CollisionSystem.h"
#include "FileSystemManager.h"
#include "FontManager.h"
#include "ResourceManager.h"
#include "DisplayString.h"
#include "InputManager.h"

#include "RenderSystem.h"
#include "LogSystem.h"
#include "ServiceLocator.h"

ServiceLocator* Engine::g_service_locator = nullptr;

Engine::Engine() :
    m_window(nullptr)
{}

Engine::~Engine()
{
    initial = false;
    Destroy();
}

ServiceLocator* Engine::getServiceLocator()
{
    return g_service_locator;
}

bool Engine::Init(const std::string& init_path, ServiceLocator* service_locator)
{
    if (!glfwInit())
    {
        glDebugMessageInsert(GL_DEBUG_SOURCE_APPLICATION, GL_DEBUG_TYPE_ERROR, 0,
            GL_DEBUG_SEVERITY_MEDIUM, -1, "Failed to initialize GLFW");
        return false;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    //glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef _DEBUG
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
#endif

    if (!service_locator)
        g_service_locator = new ServiceLocator;
    else
        g_service_locator = service_locator;

    /* Create a windowed mode window and its OpenGL context */
    g_service_locator->init(init_path);
    m_window = glfwCreateWindow(CAMERA->getActiveWindowRect().mWidth, CAMERA->getActiveWindowRect().mHeight, "ECS_GE_demo", nullptr, nullptr);
    if (!m_window)
    {
        glfwTerminate();
        return false;
    }
    /* Make the window's context current */

    glfwMakeContextCurrent(m_window);


    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        glDebugMessageInsert(GL_DEBUG_SOURCE_APPLICATION, GL_DEBUG_TYPE_ERROR, 0,
            GL_DEBUG_SEVERITY_MEDIUM, -1, "Failed to initialize OpenGL context");
        return false;
    }
    LOG("Render: " + RENDER->getRendererStr());
    LOG("OpenGL version: " + RENDER->getVersionStr());

    GLint ContextFlags;
    glGetIntegerv(GL_CONTEXT_FLAGS, &ContextFlags);
    if (ContextFlags & GL_CONTEXT_FLAG_DEBUG_BIT)
    {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(LOGER->glDebugOutput, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
    }

    FONT->setFont("arial");
    RENDER->setClearColor(1, 1, 1, 1);
    RENDER->setBlendMode(true);
    RENDER->setDepthTest(true);
    glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    if (!RES->loadResJSON("res/res.json"))
    {
        glDebugMessageInsert(GL_DEBUG_SOURCE_APPLICATION, GL_DEBUG_TYPE_ERROR, 0,
            GL_DEBUG_SEVERITY_MEDIUM, -1, "Can't load main res file. Terminate!");
        glfwTerminate();
        return false;
    }
    CAMERA->Init(m_window, { 0., 0., CAMERA->getActiveWindowRect().mWidth, CAMERA->getActiveWindowRect().mHeight });
    glfwSetWindowFocusCallback(m_window, CAMERA->glfwWindowFocusCallback);
    glfwSetCursorPosCallback(m_window, INPUTS->ÑursorPositionCallback);
    glfwSetKeyCallback(m_window, INPUTS->KeyCallback);
    glfwSetMouseButtonCallback(m_window, INPUTS->MouseButtonCallback);
    glfwSetScrollCallback(m_window, INPUTS->ScrollCallback);
    glDebugMessageInsert(GL_DEBUG_SOURCE_APPLICATION, GL_DEBUG_TYPE_ERROR, 0,
        GL_DEBUG_SEVERITY_NOTIFICATION, -1, "Init engine is ok");
    g_service_locator->postInit();
    initial = true;
    return true;
}

void Engine::Destroy() noexcept
{
    delete g_service_locator;
    g_service_locator = nullptr;
    glfwTerminate();
}

int Engine::DispatchEvents()
{
    const int ret_cod = glfwWindowShouldClose(m_window);
	/* Poll for and process events */
    if(!ret_cod)
		glfwPollEvents();
    return ret_cod;
}

void Engine::SwapBuffers()
{
    /* Swap front and back buffers */
    glfwSwapBuffers(m_window);
}