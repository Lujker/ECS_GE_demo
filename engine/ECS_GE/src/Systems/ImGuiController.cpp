#include "ImGuiController.h"

#include "imgui.h"
#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_glfw.h"
#include "Engine.h"
#include "CameraManager.h"

void ImGuiController::DrawMetricInfo()
{
    ImGuiIO& io = ImGui::GetIO();
    io.DisplaySize.x = CAMERA->getActiveWindowRect().mWidth;
    io.DisplaySize.y = CAMERA->getActiveWindowRect().mHeight;

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    if (!ImGui::Begin("Metric Info"))
    {
        ImGui::End();
        return;
    }

    // Basic info
    ImGui::Text("Dear ImGui version %s", ImGui::GetVersion());
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
    ImGui::Text("%d vertices, %d indices (%d triangles)", io.MetricsRenderVertices, io.MetricsRenderIndices, io.MetricsRenderIndices / 3);
    ImGui::Text("%d visible windows, %d active allocations", io.MetricsRenderWindows, io.MetricsActiveAllocations);

    ImGui::Separator();

    ImGui::Text("Press F1 for change cursor input mode now is: %s", 
        CAMERA->camPos.cursorInputeMode == CameraManager::CursorInputMode::eNormal? "Normal" : 
        CAMERA->camPos.cursorInputeMode == CameraManager::CursorInputMode::eHiden? "Hiden" : "Disable");
    ImGui::Text("Camera position: x: %.2f , y: %.2f, z: %.2f", 
        CAMERA->camPos.cameraPos.x, 
        CAMERA->camPos.cameraPos.y, 
        CAMERA->camPos.cameraPos.z);

    ImGui::End();
    //ImGui::ShowDemoWindow();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ImGuiController::DrawController()
{
    DrawMetricInfo();
}

void ImGuiController::init(GLFWwindow* main_window)
{
    if (isInit)
        return;
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplOpenGL3_Init();
	ImGui_ImplGlfw_InitForOpenGL(main_window, true); //prev callback not reset, and save for call before imgui callbacks
	isInit = true;
}

void ImGuiController::destroy()
{
    if (!isInit)
        return;
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	isInit = false;
}

ImGuiController::~ImGuiController()
{
	destroy();
}
