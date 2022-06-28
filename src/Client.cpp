#include "Client.h"
#include "Collision.h"
#include "Engine.h"
#include <Components/Position.h>

#include "CollisionSystem.h"
#include "DisplayString.h"
#include "LogSystem.h"
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
    UPDATE.GlobalUpdate(true);
    while (!WIDGET.GetEngine()->DispatchEvents())
    {
        WIDGET.Update(UPDATE.GlobalUpdate());
        /* Render here */
        RENDER.clear();
        WIDGET.Draw();
        WIDGET.GetEngine()->SwapBuffers();
    }
    return true;
}
