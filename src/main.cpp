#include <iostream>
#include <chrono>

#include "CameraManager.h"
#include "DisplayString.h"
#include "FileSystemManager.h"
#include "FontManager.h"
#include "ResourceManager.h"
#include "Sprite.h"
#include "Systems/RenderSystem.h"
#include "Systems/UpdateSystem.h"
#include "gl_Include.h"


int main(int argc, char** argv)
{
    /* Initialize the library */
    if (!glfwInit())
    {
        std::cout << "Failed to initialize GLFW" << std::endl;
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    CAMERA.setActiveWindowSize(640, 420);
    GLFWwindow* window = glfwCreateWindow(CAMERA.getActiveWindowRect().mWidth, CAMERA.getActiveWindowRect().mHeight, "ECS_GE_demo", nullptr, nullptr);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to initialize OpenGL context" << std::endl;
        return -1;
    }
    std::cout << "Render: " << RENDER.getRendererStr() << std::endl;
    std::cout << "OpenGL version: " << RENDER.getVersionStr() << std::endl;
    FONT.init();
    FONT.setFont("arial");
    glfwSetWindowSizeCallback(window, CAMERA.glfwWindowsSizeCallback);
    RENDER.setClearColor(1, 1, 1, 1);
    RENDER.setBlendMode(true);
    RENDER.setDepthTest(true);
    FILES.setExecutablePath(argv[0]);
    RES.setExecutablePath(argv[0]);
	if (!RES.loadResJSON("res\\res.json"))
	{
        std::cerr << "Can't load main res file. Terminate!" << std::endl;
        glfwTerminate();
        return 0;
	}
    CAMERA.glfwWindowsResize(window, 1024, 640);
    CAMERA.Init({ 0., 0., CAMERA.getActiveWindowRect().mWidth, CAMERA.getActiveWindowRect().mHeight });
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
        const auto string = std::make_shared<DisplayString>("Cant");
        string->mirror(false, false);
        sprite->setAnimation("idle");
        for (auto i : sprite->getAnimationsName())
            std::cout << i << std::endl;
        /* Loop until the user closes the window */

        UPDATE.GlobalUpdate(true);
        while (!glfwWindowShouldClose(window))
        {
            /* Poll for and process events */
            glfwPollEvents();
            UPDATE.GlobalUpdate();
        	UPDATE.Update(sprite);
            /* Render here */
            RENDER.clear();
        	RENDER.Render(sprite, pos_sprite, size_sprite);
            RENDER.Render(image, pos_img, col_img);
            RENDER.Render(FRect{ pos_sprite.getPosition().mX + col_sprite.getXOffset(), pos_sprite.getPosition().mY + col_sprite.getYOffset(), col_sprite.getWidth(), col_sprite.getHeight()});
            RENDER.Render(string, string_pos,1.f, {0.3f,1.f,0.5f, 0.6f});
            /* Swap front and back buffers */
            glfwSwapBuffers(window);
        }
        RES.unloadAllResources();
        FILES.unloadAllFiles();
        CAMERA.ReleaseShader();
        FONT.freeFontRes();
    }
    glfwTerminate();
    return 0;
}
