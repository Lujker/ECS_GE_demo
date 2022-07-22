#include "WidgetsManager.h"

#include "GlobalWidget.h"
#include "LogSystem.h"
#include "Widget.h"

WidgetManager& WidgetManager::Instanse()
{
	static WidgetManager widget_manager;
	return widget_manager;
}

std::shared_ptr<Widget> WidgetManager::GetUI()
{
	return Instanse().m_UI_Widget;
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
	exit(0);
}

void WidgetManager::Close()
{
	m_closed = true;
}

bool WidgetManager::SholdBeClosed()
{
	return m_closed;
}

//!Change global widget with anim
//!Next window start show with anim, after use callback AddedToContainer
//!After showed new widget old window closed and use removedFromContainer callback and removed without animation
void WidgetManager::SetNextWidget(const std::shared_ptr<GlobalWidget>& nextWindow)
{
	if (m_pNextWidget)
		return;
	m_pNextWidget = nextWindow;
	globalWindowsChangAnim = true;
	auto setNextWidget = [&]()
	{
		m_pNextWidget->OriginRectSet();
		m_pNextWidget->AddedToContainer(nullptr);
		m_pNextWidget->StartShow([&]()
			{
				std::swap(m_pFocusWidget, m_pNextWidget);
				globalWindowsChangAnim = false;
				m_pNextWidget.reset();
			}, true);
	};
	auto closeOldWidget = [&]()
	{
		LOG("Remove widget: " + m_pFocusWidget->GetName() + " from GLOBAL MANAGER!");
		m_pFocusWidget->RemovedFromContainer(nullptr);
		m_pFocusWidget->StartClose([&, setNextWidget]()
			{
				LOG("Widget: " + m_pFocusWidget->GetName() + " removed!");
				LOG("Push widget: " + m_pNextWidget->GetName() + " to GLOBAL MANAGER!");
				setNextWidget();
			}, true);
	};
	if (m_pNextWidget)
	{
		if (m_pFocusWidget)
			closeOldWidget();
		else
			setNextWidget();
	}
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
	{
		Transform2DGuard t_r{ m_pNextWidget->mTransform };
		m_pNextWidget->DrawAll();
	}
	if (m_pFocusWidget)
	{
		Transform2DGuard t_r{ m_pFocusWidget->mTransform };
		m_pFocusWidget->DrawAll();
	}
}
