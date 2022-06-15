#include <iostream>
#include <chrono>

#include "CameraManager.h"
#include "FileSystemManager.h"
#include "ResourceManager.h"
#include "Sprite.h"
#include "Systems/RenderSystem.h"
#include "Systems/UpdateSystem.h"
#include <ft2build.h>
#include FT_FREETYPE_H


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

    FT_Library ft;
    if (FT_Init_FreeType(&ft))
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;

    glfwSetWindowSizeCallback(window, CAMERA.glfwWindowsSizeCallback);
    RENDER.setClearColor(1, 1, 1, 1);
    RENDER.setBlendMode(true);
    RENDER.setDepthTest(true);
    FILES.setExecutablePath(argv[0]);
    RES.setExecutablePath(argv[0]);
	if (!RES.loadResJSON("res\\res.json"))
	{
        glfwTerminate();
        return 0;
	}
    CAMERA.glfwWindowsResize(window, 1024, 640);
    CAMERA.Init({ 0., 0., CAMERA.getActiveWindowRect().mWidth, CAMERA.getActiveWindowRect().mHeight });
    CAMERA.SetShader(RES.getShader("image_shader"));
    CAMERA.Update();

    CAMERA.SetShader(RES.getShader("default"));
    CAMERA.Update();
    {
    	const PositionComponent pos_img{ 100, 100 };
		const CollisionComponent col_img{ 100, 100, false };
        const PositionComponent pos_sprite{ 50, 100 };
        const CollisionComponent size_sprite{ 120, 80, false };
        const CollisionComponent col_sprite{ 35, 40, 55, 0, false };
		const auto image = RES.getSharedImage("res/textures/tank.png");
        const auto sprite = RES.getSprite("res/sprites/test/Kinght");
        //sprite->setAnimation("idle");
        for (auto i : sprite->getAnimationsName())
            std::cout << i << std::endl;
        /* Loop until the user closes the window */

        UPDATE.GlobalUpdate(true);
        while (!glfwWindowShouldClose(window))
        {
            /* Poll for and process events */
            glfwPollEvents();
            UPDATE.GlobalUpdate();
            if(sprite)
				UPDATE.Update(sprite);
            /* Render here */
            RENDER.clear();
            if (sprite)
                RENDER.Render(sprite, pos_sprite, size_sprite);
            RENDER.Render(image, pos_img, col_img);
            RENDER.Render(FRect{ pos_sprite.getPosition().mX + col_sprite.getXOffset(), pos_sprite.getPosition().mY + col_sprite.getYOffset(), col_sprite.getWidth(), col_sprite.getHeight()});
            /* Swap front and back buffers */
            glfwSwapBuffers(window);
        }
        RES.unloadAllResources();
        FILES.unloadAllFiles();
        CAMERA.ReleaseShader();
    }
    glfwTerminate();
    return 0;
}
