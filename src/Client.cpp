#include "Client.h"
#include "Collision.h"
#include <Components/Position.h>

#include "CameraManager.h"
#include "CollisionSystem.h"
#include "DisplayString.h"
#include "FileSystemManager.h"
#include "LogSystem.h"
#include "MainMenuWindow.h"
#include "PhysicsSystem.h"
#include "RenderSystem.h"
#include "ResourceManager.h"
#include "Sprite.h"
#include "UpdateSystem.h"
#include "WidgetsManager.h"
#include "Windows/SandboxWindow.h"
#include "ImGuiController.h"
#include "LightManager.h"

Client::Client(const char* argv):
	m_exec_path(argv)
{
	init();
}

Client::~Client()
{
    unload();
}

bool Client::unload()
{
    FILES->unloadAllFiles();
    RES->unloadAllResources();
    Terminate();
	return true;
}

bool Client::isInit() const
{
	return m_isInit;
}

bool Client::init()
{
	if (m_isInit || m_exec_path.empty())
		return false;
	if (!InitEngine())
		return false;
    m_isInit = true;
    WIDGET->SetNextWidget(SandboxWindow::Create());
    return true;
}

bool Client::InitEngine()
{
    m_pAppEngine = std::make_shared<Engine>();
    auto* service_locator = new DemoServiceLocator;
    m_isEngineInit = m_pAppEngine->Init(m_exec_path, service_locator);
    return m_isEngineInit;
}

bool Client::IsEngineInit() const
{
    return m_isEngineInit;
}

std::shared_ptr<Engine> Client::GetEngine() const
{
    return m_pAppEngine;
}

void Client::Terminate()
{
    if (m_pAppEngine)
        m_pAppEngine.reset();
    exit(0);
}


bool Client::deltaLoop()
{
    constexpr float freq = 1000.0f / 60.0f; //60 FPS lock 
    UPDATE->GlobalUpdate(true);
    while (!m_pAppEngine->DispatchEvents() && !WIDGET->SholdBeClosed())
    {
        const unsigned long startMS = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
        const float delta_time = UPDATE->GlobalUpdate();
        if (delta_time > 0.f)
        {
            PHYSICS->Update(delta_time);
            CAMERA->UpdateMoveCamera(delta_time);
            WIDGET->Update(delta_time);
            /* Render here */
            RENDER->clear();
            WIDGET->Draw();
            LIGHT->Draw();
            drawDebug();
            m_pAppEngine->SwapBuffers();
        }
        const unsigned long endMS = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count() - startMS;
        UPDATE->Pause(freq - endMS);
    }
    return true;
}

void Client::drawDebug()
{
    if(CAMERA->isDrawDebugInfo())
    {
        UI->DrawController();
    }
}
