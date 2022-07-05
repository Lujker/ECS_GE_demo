#pragma once
#include "GameObject.h"
#include "Move.h"
#include "Sprite.h"


class Knight : public IGameObject, public RenderEngine::SpriteAnimListener
{
public:
	Knight(const std::string& name, const PositionComponent& default_position, const CollisionComponent& default_size, const CollisionComponent& default_collision);
	~Knight() override;
	void Init() override;
	void Draw() override;
	void Update(const float& delta_time) override;
	void KeyPress(const int& key) override;
	void KeyUnpress(const int& key) override;
	void MoveChange() override;
	void PositionChange() override;
	void IsAnimationEnd(const std::string& anim) override;
	void IsAnimationStart(const std::string& anim) override;
private:
	std::shared_ptr<RenderEngine::Sprite> m_sprite;
};
