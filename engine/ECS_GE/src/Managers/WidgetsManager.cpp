#include "WidgetsManager.h"

#include "GlobalWidget.h"
#include "LogSystem.h"
#include "Widget.h"

WidgetManager& WidgetManager::Instanse()
{
	static WidgetManager widget_manager;
	return widget_manager;
}

bool WidgetManager::InitEngine(std::string execPath)
{
	m_pAppEngine = std::make_shared<Engine>();
	m_execPath = std::move(execPath);
	m_isEngineInit = m_pAppEngine->Init(m_execPath);
	return m_isEngineInit;
}

bool WidgetManager::IsEngineInit() const
{
	return m_isEngineInit;
}

std::shared_ptr<Engine> WidgetManager::GetEngine() const
{
	return m_pAppEngine;
}

void WidgetManager::Terminate()
{
	if (m_pAppEngine)
		m_pAppEngine.reset();
}

//!Change global widget with anim
//!Next window start show with anim, after use callback AddedToContainer
//!After showed new widget old window closed and use removedFromContainer callback and removed without animation
void WidgetManager::SetNextWidget(const std::shared_ptr<GlobalWidget>& nextWindow)
{
	m_pNextWidget = nextWindow;
	globalWindowsChangAnim = true;
	m_pNextWidget->StartShow([&]()
		{
			if (m_pNextWidget)
			{
				m_pNextWidget->OriginRectSet();
				m_pNextWidget->AddedToContainer(nullptr);
			}
			else 
				return;
			LOG("Push widget: " + m_pNextWidget->GetName() + " to GLOBAL MANAGER!");
			if (m_pFocusWidget)
			{
				m_pFocusWidget->StartClose([&]()
					{
						if (m_pFocusWidget)
							m_pFocusWidget->RemovedFromContainer(nullptr);
						LOG("Remove widget: " + m_pFocusWidget->GetName() + " from GLOBAL MANAGER!");
						std::swap(m_pFocusWidget, m_pNextWidget);
						globalWindowsChangAnim = false;
					}, false);
			}
			else
			{
				std::swap(m_pFocusWidget, m_pNextWidget);
				globalWindowsChangAnim = false;
			}
		}, true);


}

void WidgetManager::Update(float deltaTime)
{
	if (m_pNextWidget)
		m_pNextWidget->UpdateAll(deltaTime);
	if (m_pFocusWidget)
		m_pFocusWidget->UpdateAll(deltaTime);
}

void WidgetManager::Draw()
{
	if (m_pNextWidget)
		m_pNextWidget->DrawAll();
	if (m_pFocusWidget)
		m_pFocusWidget->DrawAll();
}
