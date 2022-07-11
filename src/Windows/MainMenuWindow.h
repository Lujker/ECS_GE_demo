#pragma once
#include "GlobalWidget.h"
#include "Button.h"

class MainMenuWindow : public GlobalWidget, public ButtonListener
{
public:
	static std::shared_ptr<MainMenuWindow> Create();
	MainMenuWindow();
	~MainMenuWindow() override = default;

	void OriginRectSet() override;
	void AddedToContainer(std::shared_ptr<WidgetContainer> theWidgetContainer) override;
	void RemovedFromContainer(std::shared_ptr<WidgetContainer> theWidgetContainer) override;
	void Draw() override;
	void Update(float deltaTime) override;
	//void KeyPress(const int& key) override;
	//void KeyUnpress(const int& key) override;
	//void MouseMove(const FPoint& current_pos) override;
	//void MousePress(const int& key) override;
	//void MouseUnpress(const int& key) override;

	void ButtonPressed(unsigned id, const int& key) override;
	void ButtonUnpressed(unsigned id, const int& key) override;
private:
	std::list<std::shared_ptr<IInterfaceObject>> m_objects;
};
