#pragma once
#include <memory>

#include "Engine.h"
#include "WidgetContainer.h"


class GlobalWidget;

class WidgetManager : public InputListener
{
	std::shared_ptr<GlobalWidget>		m_pFocusWidget = nullptr;
	std::shared_ptr<GlobalWidget>		m_pNextWidget = nullptr;
	std::shared_ptr<GlobalWidget>		m_pLoadingScreen = nullptr;
	std::shared_ptr<Widget>				m_UI_Widget = nullptr;
	std::string							m_execPath;
	bool								globalWindowsChangAnim = false;
	bool								m_closed = false;
public:
	friend class ServiceLocator;
	std::shared_ptr<Widget> GetUI();

	//Engine
	void					Close();
	bool					SholdBeClosed();

	void	SetNextWidget(const std::shared_ptr<GlobalWidget>& nextWindow);
	void	Update(float deltaTime);
	void	Draw();

	WidgetManager(const WidgetManager&) = delete;
	WidgetManager(WidgetManager&&) = delete;
	WidgetManager& operator=(const WidgetManager&) = delete;
	WidgetManager& operator=(WidgetManager&&) = delete;
private:
	WidgetManager() = default;
	~WidgetManager() override = default;
};
