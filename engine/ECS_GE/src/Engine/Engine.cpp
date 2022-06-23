#include "Engine.h"

#include "CameraManager.h"
#include "CollisionSystem.h"
#include "FileSystemManager.h"
#include "FontManager.h"
#include "ResourceManager.h"
#include "DisplayString.h"

#include "RenderSystem.h"
#include "LogSystem.h"

Engine::Engine() :
    m_window(nullptr)
{}

Engine::~Engine()
{
    Destroy();
}

bool Engine::Init(const std::string& init_path)
{
    if (!glfwInit())
    {
        glDebugMessageInsert(GL_DEBUG_SOURCE_APPLICATION, GL_DEBUG_TYPE_ERROR, 0,
            GL_DEBUG_SEVERITY_MEDIUM, -1, "Failed to initialize GLFW");
        return false;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef _DEBUG
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
#endif

    /* Create a windowed mode window and its OpenGL context */
    CAMERA.setActiveWindowSize(1310, 768);
    LOGER.init(init_path);
	FILES.setExecutablePath(init_path);
    RES.setExecutablePath(init_path);
    FONT.init(init_path);
    m_window = glfwCreateWindow(CAMERA.getActiveWindowRect().mWidth, CAMERA.getActiveWindowRect().mHeight, "ECS_GE_demo", nullptr, nullptr);
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
    LOG("Render: " + RENDER.getRendererStr());
    LOG("OpenGL version: " + RENDER.getVersionStr());

    GLint ContextFlags;
    glGetIntegerv(GL_CONTEXT_FLAGS, &ContextFlags);
    if (ContextFlags & GL_CONTEXT_FLAG_DEBUG_BIT)
    {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(LOGER.glDebugOutput, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
    }

    FONT.setFont("arial");
    RENDER.setClearColor(1, 1, 1, 1);
    RENDER.setBlendMode(true);
    RENDER.setDepthTest(true);
    if (!RES.loadResJSON("res/res.json"))
    {
        glDebugMessageInsert(GL_DEBUG_SOURCE_APPLICATION, GL_DEBUG_TYPE_ERROR, 0,
            GL_DEBUG_SEVERITY_MEDIUM, -1, "Can't load main res file. Terminate!");
        glfwTerminate();
        return false;
    }
    //CAMERA.glfwWindowsResize(m_window, 1310, 768);
    CAMERA.Init({ 0., 0., CAMERA.getActiveWindowRect().mWidth, CAMERA.getActiveWindowRect().mHeight });
    glfwSetWindowFocusCallback(m_window, CAMERA.glfwWindowFocusCallback);
    glDebugMessageInsert(GL_DEBUG_SOURCE_APPLICATION, GL_DEBUG_TYPE_ERROR, 0,
        GL_DEBUG_SEVERITY_NOTIFICATION, -1, "Init engine is ok");
    return true;
}

void Engine::Destroy() noexcept
{
    RES.unloadAllResources();
    FILES.unloadAllFiles();
    CAMERA.ReleaseShader();
    FONT.freeFontRes();
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