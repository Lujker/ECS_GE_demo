#pragma once
#include "GameObject.h"
#include "Image.h"

class Board : public IGameObject
{
public:
	Board(const std::string& name, const PositionComponent& default_position,
		const CollisionComponent& default_size, const CollisionComponent& default_collision);
	~Board() override = default;
	void Init() override;
	void Draw() override;
	void Update(const float& delta_time) override;
private:
	std::shared_ptr<RenderEngine::Image2D>	m_image;
};
