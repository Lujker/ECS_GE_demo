#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <chrono>

#include "FileSystemManager.h"
#include "ResourceManager.h"
#include "Systems/RenderSystem.h"

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
    GLFWwindow* window = glfwCreateWindow(640, 420, "ECS_GE_demo", nullptr, nullptr);
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

    RENDER.setClearColor(0, 1, 0, 1);
    RENDER.setBlendMode(true);
    RENDER.setDepthTest(true);
    FILES.setExecutablePath(argv[0]);
    RES.setExecutablePath(argv[0]);

    {
        auto lastTime = std::chrono::high_resolution_clock::now();
        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Poll for and process events */
            glfwPollEvents();
            auto currentTime = std::chrono::high_resolution_clock::now();
            float duration = std::chrono::duration<float, std::milli>(currentTime - lastTime).count();
            lastTime = currentTime;
            /* Render here */
            RENDER.clear();
            /* Swap front and back buffers */
            glfwSwapBuffers(window);
        }
        RES.unloadAllResources();
        FILES.unloadAllFiles();
    }
    glfwTerminate();
    return 0;
}
