#include "Client.h"
#include "Collision.h"
#include "Engine.h"
#include <Components/Position.h>

#include "CameraManager.h"
#include "CollisionSystem.h"
#include "DisplayString.h"
#include "FileSystemManager.h"
#include "LogSystem.h"
#include "PhysicsSystem.h"
#include "RenderSystem.h"
#include "ResourceManager.h"
#include "Sprite.h"
#include "UpdateSystem.h"
#include "WidgetsManager.h"
#include "SandboxWindow.h"

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
    FILES.unloadAllFiles();
    RES.unloadAllResources();
    WIDGET.Terminate();
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
	if (!WIDGET.InitEngine(m_exec_path))
		return false;
    m_isInit = true;
    WIDGET.SetNextWidget(SandboxWindow::Create());
    return true;
}

bool Client::deltaLoop()
{
    constexpr float freq = 1000.0f / 60.0f; //60 FPS lock 
    UPDATE.GlobalUpdate(true);
    while (!WIDGET.GetEngine()->DispatchEvents())
    {
        const unsigned long startMS = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
        const float delta_time = UPDATE.GlobalUpdate();
        if (delta_time > 0.f)
        {
            PHYSICS.Update(delta_time);
            WIDGET.Update(delta_time);
            /* Render here */
            RENDER.clear();
            WIDGET.Draw();
            WIDGET.GetEngine()->SwapBuffers();
        }
        const unsigned long endMS = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count() - startMS;
        UPDATE.Pause(freq - endMS);
    }
    return true;
}
