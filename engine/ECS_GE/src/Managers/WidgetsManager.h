#pragma once

#include <memory>

#include "Engine.h"
#include "WidgetContainer.h"

#define WIDGET WidgetManager::Instanse()
class GlobalWidget;

class WidgetManager : public InputListener
{
	std::shared_ptr<Engine>				m_pAppEngine = nullptr;
	std::shared_ptr<GlobalWidget>		m_pFocusWidget = nullptr;
	std::shared_ptr<GlobalWidget>		m_pNextWidget = nullptr;
	std::shared_ptr<GlobalWidget>		m_pLoadingScreen = nullptr;
	std::string							m_execPath;
	bool								globalWindowsChangAnim = false;
public:
	static WidgetManager& Instanse();

	//Engine
	void					InitEngine(std::string	execPath);
	std::shared_ptr<Engine> GetEngine() const;
	void					Terminate();

	void	SetNextWidget(const std::shared_ptr<GlobalWidget>& nextWindow);
	void	Update(float deltaTime);
	void	Draw();
private:
	WidgetManager() = default;
	~WidgetManager() = default;
	WidgetManager(const WidgetManager&) = delete;
	WidgetManager(WidgetManager&&) = delete;
	WidgetManager& operator=(const WidgetManager&) = delete;
	WidgetManager& operator=(WidgetManager&&) = delete;
};
