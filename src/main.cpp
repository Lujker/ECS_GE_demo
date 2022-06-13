#include <iostream>
#include <chrono>

#include "CameraManager.h"
#include "FileSystemManager.h"
#include "ResourceManager.h"
#include "Systems/RenderSystem.h"
#include "Systems/UpdateSystem.h"


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

    glfwSetWindowSizeCallback(window, CAMERA.glfwWindowsSizeCallback);
    RENDER.setClearColor(0, 0, 0, 1);
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
    {
    	const PositionComponent pos{ 100, 100 };
		const CollisionComponent col{ 100, 100, false };
		const auto image = RES.getSharedImage("res/textures/tank.png");
        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Poll for and process events */
            glfwPollEvents();
            UPDATE.GlobalUpdate();
            /* Render here */
            RENDER.clear();
            RENDER.Render(*image, pos, col);
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
