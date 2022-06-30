#pragma once
#include "Collision.h"
#include "GlobalWidget.h"
#include "Knight.h"
#include "Position.h"

class DisplayString;

namespace RenderEngine
{
	class Sprite;
	class Image2D;
}

class SandboxWindow : public GlobalWidget
{
public:
	static std::shared_ptr<SandboxWindow> Create();
	SandboxWindow();
	~SandboxWindow() override = default;

	void OriginRectSet() override;
	void AddedToContainer(std::shared_ptr<WidgetContainer> theWidgetContainer) override;
	void RemovedFromContainer(std::shared_ptr<WidgetContainer> theWidgetContainer) override;
	void Draw() override;
	void Update(float deltaTime) override;
	void KeyPress(const int& key) override;
	void KeyUnpress(const int& key) override;
private:
	std::shared_ptr<RenderEngine::Image2D> m_bg;
	std::shared_ptr<Knight> m_knight;
};
