#pragma once
#include "GameObject.h"
#include "GlobalWidget.h"

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
	std::list< std::shared_ptr<IGameObject>> m_objects;
};
