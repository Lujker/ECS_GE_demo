#include "Client.h"
#include <iostream>
#include "Windows.h"

#include "CameraManager.h"
#include "FileSystemManager.h"
#include "FontManager.h"
#include "ResourceManager.h"
#include "Sprite.h"
#include "DisplayString.h"

#include "RenderSystem.h"
#include "UpdateSystem.h"
void APIENTRY glDebugOutput(GLenum source,
    GLenum type,
    GLuint id,
    GLenum severity,
    GLsizei length,
    const GLchar* message,
    const void* userParam);

Client::Client():
    m_window(nullptr)
{}

Client::~Client()
{
    Destroy();
}

bool Client::Init(const std::string& init_path)
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
    std::cout << "Render: " << RENDER.getRendererStr() << std::endl;
    std::cout << "OpenGL version: " << RENDER.getVersionStr() << std::endl;

    GLint ContextFlags;
    glGetIntegerv(GL_CONTEXT_FLAGS, &ContextFlags);
    if (ContextFlags & GL_CONTEXT_FLAG_DEBUG_BIT)
    {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(glDebugOutput, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
    }


    FONT.init();
    FONT.setFont("arial");
    RENDER.setClearColor(1, 1, 1, 1);
    RENDER.setBlendMode(true);
    RENDER.setDepthTest(true);
    FILES.setExecutablePath(init_path);
    RES.setExecutablePath(init_path);
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
        GL_DEBUG_SEVERITY_NOTIFICATION, -1, "Init clint is ok");
	return true;
}

void Client::Destroy() noexcept
{
    RES.unloadAllResources();
    FILES.unloadAllFiles();
    CAMERA.ReleaseShader();
    FONT.freeFontRes();
    glfwTerminate();
}

void Client::DeltaLoop()
{
    const PositionComponent pos_img{ 100, 100 };
    const CollisionComponent col_img{ 100, 100, false };
    const PositionComponent pos_sprite{ 350, 100 };
    const CollisionComponent size_sprite{ 120, 80, false };
    const CollisionComponent col_sprite{ 40, 40, 40, 0, false };
    const PositionComponent string_pos{ 300,300, 2 };
    const auto atlas = RES.getFrameAtlas("res/atlas/_AttackNoMovement");
    const auto imaga_atlas = std::make_shared<RenderEngine::Image2D>(atlas);
    imaga_atlas->SetSubTexture(atlas->getFrame("1")->second);
    const auto image = RES.getSharedImage("res/images/tank.png");
    const auto sprite = RES.getSprite("res/sprites/test/Kinght");
    sprite->mirror(false, false);
    const auto string = std::make_shared<DisplayString>("Can't");
    string->mirror(false, false);
    //sprite->setAnimation("idle");
    for (auto i : sprite->getAnimationsName())
        std::cout << i << std::endl;
    /* Loop until the user closes the window */

    UPDATE.GlobalUpdate(true);
    while (!glfwWindowShouldClose(m_window))
    {
        /* Poll for and process events */
        glfwPollEvents();
        UPDATE.GlobalUpdate();
        UPDATE.Update(sprite);
        /* Render here */
        RENDER.clear();
        RENDER.Render(sprite, pos_sprite, size_sprite);
        RENDER.Render(image, pos_img + FPoint{100, 100}, col_img);
        RENDER.Render(FRect{ pos_sprite.getPosition().mX + col_sprite.getXOffset(), pos_sprite.getPosition().mY + col_sprite.getYOffset(), col_sprite.getWidth(), col_sprite.getHeight() });
        RENDER.Render(string, string_pos, 1.f, { 0.3f,1.f,0.5f, 0.6f });
        RENDER.Render(imaga_atlas, pos_img, size_sprite);
        /* Swap front and back buffers */
        glfwSwapBuffers(m_window);
    }
}
//!TODO need Loger
void APIENTRY glDebugOutput(GLenum source,
    GLenum type,
    GLuint id,
    GLenum severity,
    GLsizei length,
    const GLchar* message,
    const void* userParam)
{
    // ignore non-significant error/warning codes
    if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;
    static std::map<GLenum, const GLchar*> Sources =
    {
        {GL_DEBUG_SOURCE_API, "API"},
        {GL_DEBUG_SOURCE_WINDOW_SYSTEM, "WINDOW_SYSTEM"},
        {GL_DEBUG_SOURCE_SHADER_COMPILER, "SHADER_COMPILER"},
        {GL_DEBUG_SOURCE_THIRD_PARTY, "THIRD_PARTY"},
        {GL_DEBUG_SOURCE_APPLICATION, "APPLICATION"},
        {GL_DEBUG_SOURCE_OTHER, "OTHER"}
    };

    static std::map<GLenum, const GLchar*> Severities =
    {
        {GL_DEBUG_SEVERITY_HIGH, "HIGH"},
        {GL_DEBUG_SEVERITY_MEDIUM, "MEDIUM"},
        {GL_DEBUG_SEVERITY_LOW, "LOW"},
        {GL_DEBUG_SEVERITY_NOTIFICATION, "NOTIFICATION"}
    };

    static std::map<GLenum, const GLchar*> Types =
    {
        {GL_DEBUG_TYPE_ERROR, "ERROR"},
        {GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR, "DEPRECATED_BEHAVIOR"},
        {GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR, "UNDEFINED_BEHAVIOR"},
        {GL_DEBUG_TYPE_PORTABILITY, "PORTABILITY"},
        {GL_DEBUG_TYPE_PERFORMANCE, "PERFORMANCE"},
        {GL_DEBUG_TYPE_MARKER, "MARKER"},
        {GL_DEBUG_TYPE_PUSH_GROUP, "PUSH_GROUP"},
        {GL_DEBUG_TYPE_POP_GROUP, "POP_GROUP"},
        {GL_DEBUG_TYPE_OTHER, "OTHER"}
    };
    char ErrorString[512];
	sprintf_s(ErrorString, "[OpenGL %s] - SEVERITY: %s, SOURCE: %s, ID: %d: %s\n", Types[type], Severities[severity], Sources[source], id, message);
    //printf("[OpenGL %s] - SEVERITY: %s, SOURCE: %s, ID: %d: %s\n", Types[type], Severities[severity], Sources[source], id, message);
    OutputDebugStringA(ErrorString);
}