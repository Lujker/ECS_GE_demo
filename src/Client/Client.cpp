#include "Client.h"

#include <iostream>
#include "CameraManager.h"
#include "FileSystemManager.h"
#include "FontManager.h"
#include "ResourceManager.h"
#include "Sprite.h"
#include "DisplayString.h"

#include "RenderSystem.h"
#include "UpdateSystem.h"

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
        std::cout << "Failed to initialize GLFW" << std::endl;
        return false;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    CAMERA.setActiveWindowSize(640, 420);
    m_window = glfwCreateWindow(CAMERA.getActiveWindowRect().mWidth, CAMERA.getActiveWindowRect().mHeight, "ECS_GE_demo", nullptr, nullptr);
    if (!m_window)
    {
        glfwTerminate();
        return false;
    }
    /* Make the window's context current */
    glfwMakeContextCurrent(m_window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize OpenGL context" << std::endl;
        return false;
    }
    std::cout << "Render: " << RENDER.getRendererStr() << std::endl;
    std::cout << "OpenGL version: " << RENDER.getVersionStr() << std::endl;
    FONT.init();
    FONT.setFont("arial");
    RENDER.setClearColor(1, 1, 1, 1);
    RENDER.setBlendMode(true);
    RENDER.setDepthTest(true);
    FILES.setExecutablePath(init_path);
    RES.setExecutablePath(init_path);
    if (!RES.loadResJSON("res\\res.json"))
    {
        std::cerr << "Can't load main res file. Terminate!" << std::endl;
        glfwTerminate();
        return false;
    }
    CAMERA.glfwWindowsResize(m_window, 1310, 768);
    CAMERA.Init({ 0., 0., CAMERA.getActiveWindowRect().mWidth, CAMERA.getActiveWindowRect().mHeight });

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
        RENDER.Render(image, pos_img, col_img);
        RENDER.Render(FRect{ pos_sprite.getPosition().mX + col_sprite.getXOffset(), pos_sprite.getPosition().mY + col_sprite.getYOffset(), col_sprite.getWidth(), col_sprite.getHeight() });
        RENDER.Render(string, string_pos, 1.f, { 0.3f,1.f,0.5f, 0.6f });
        /* Swap front and back buffers */
        glfwSwapBuffers(m_window);
    }
}
