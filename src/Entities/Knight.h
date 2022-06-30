#pragma once
#include "GameObject.h"
#include "Move.h"

namespace RenderEngine
{
	class Sprite;
}

class Knight : public IGameObject
{
public:
	Knight(const std::string& name, const PositionComponent& default_position, const CollisionComponent& default_size, const CollisionComponent& default_collision);
	~Knight() override = default;
	void Init() override;
	void Draw() override;
	void Update(const float& delta_time) override;
	void KeyPress(const int& key) override;
	void KeyUnpress(const int& key) override;
private:
	MoveComponent m_move;
	std::shared_ptr<RenderEngine::Sprite> m_sprite;
};
