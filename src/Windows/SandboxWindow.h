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
	void AddedToContainer(SharedWidgetContainer theWidgetContainer) override;
	void RemovedFromContainer(SharedWidgetContainer theWidgetContainer) override;
	void Draw() override;
	void Update(float deltaTime) override;
	void KeyPress(const int& key) override;
	void KeyUnpress(const int& key) override;
private:
	GameObjectList m_objects;
};
