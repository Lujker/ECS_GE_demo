#pragma once
#include "Collision.h"
#include "GlobalWidget.h"
#include "Knight.h"
#include "Board.h"
#include "Position.h"

class DisplayString;

namespace RenderEngine
{
	class Image2D;
}

class MainMenuWindow : public GlobalWidget
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
	void KeyPress(const int& key) override;
	void KeyUnpress(const int& key) override;
	void MouseMove(const FPoint& current_pos) override;
	void MousePress(const int& key) override;
	void MouseUnpress(const int& key) override;
private:
};
