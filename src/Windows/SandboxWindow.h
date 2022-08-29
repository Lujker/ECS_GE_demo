#pragma once
#include "GameObject.h"
#include "GlobalWidget.h"
#include <Components/Cube.h>

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
	std::shared_ptr<RenderEngine::Cube> cub;
	RenderEngine::Material cub_material; /*{ {1.0f, 0.5f, 0.31f}, {1.0f, 0.5f, 0.31f}, {0.5f, 0.5f, 0.5f}, 32.0f };*/
	PositionComponent3 cub_pos{ 0,0, 80};
	CollisionComponent3 cub_size{ 50.f, 50.f, 100.f};
};
