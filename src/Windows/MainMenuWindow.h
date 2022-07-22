#pragma once
#include "GlobalWidget.h"
#include "Button.h"

class MainMenuWindow : public GlobalWidget, public ButtonListener
{
public:
	static std::shared_ptr<MainMenuWindow> Create();
	MainMenuWindow();
	~MainMenuWindow() override = default;

	enum class eButton : unsigned
	{
		eToSandbox = 0,
		eExit = 1
	};

	void OriginRectSet() override;
	void AddedToContainer(SharedWidgetContainer theWidgetContainer) override;
	void RemovedFromContainer(SharedWidgetContainer theWidgetContainer) override;
	void Draw() override;
	void Update(float deltaTime) override;

	void ButtonPressed(unsigned id, const int& key) override;
	void ButtonUnpressed(unsigned id, const int& key) override;
private:
	InterfaceObjectList m_objects;
};
