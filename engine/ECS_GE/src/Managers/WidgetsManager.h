#pragma once

#include <memory>

#include "Engine.h"
#include "WidgetContainer.h"

#define WIDGET WidgetManager::Instanse()
class WidgetManager : public WidgetContainer
{
	std::shared_ptr<Engine> m_pAppEngine;
	std::shared_ptr<Widget> m_pFocusWidget;
	std::shared_ptr<Widget> m_pLastAddedWidget;
	std::shared_ptr<Widget> m_pUserInterfaceWidget;
	float					m_layerUI;
	bool					m_isUIdraw;
	bool					m_hasFocus;
	std::string				m_execPath;
public:
	static WidgetManager& Instanse();

	//Engine
	void	InitEngine(std::string	execPath);
	std::shared_ptr<Engine> getEngine() const;

	//UI
	bool	IsUIDraw();
	void	ShowUI();
	void	HideUI();

	//WidgetConatiner
	void	AddWidget(std::shared_ptr<Widget> theWidget) override;
	void	RemoveWidget(std::shared_ptr<Widget> theWidget) override;
	bool	HasWidget(std::shared_ptr<Widget> theWidget) override;
	void	DisableWidget(std::shared_ptr<Widget> theWidget) override;
	void	RemoveAllWidgets(bool doDelete = false, bool recursive = false) override;

	//!Callback's
	void	StartShow() override;
	void	StartClose() override;
	void	AddedToContainer(std::shared_ptr<WidgetContainer> theWidgetManager) override;
	void	RemovedFromContainer(std::shared_ptr<WidgetContainer> theWidgetManager) override;

	void	Update(float deltaTime) override;
	void	UpdateAll(float deltaTime) override;
	void	Draw() override;
	void	DrawAll() override;
private:
	WidgetManager() = default;
	~WidgetManager() = default;
	WidgetManager(const WidgetManager&) = delete;
	WidgetManager(WidgetManager&&) = delete;
	WidgetManager& operator=(const WidgetManager&) = delete;
	WidgetManager& operator=(WidgetManager&&) = delete;
};