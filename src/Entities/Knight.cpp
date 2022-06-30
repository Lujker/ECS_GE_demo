#include "Knight.h"

#include "RenderSystem.h"
#include "ResourceManager.h"
#include "Sprite.h"
#include "UpdateSystem.h"

Knight::Knight(const std::string& name, const PositionComponent& default_position, const CollisionComponent& default_size, const CollisionComponent& default_collision):
	IGameObject(name,default_position,default_size,default_collision)
{
}

void Knight::Init()
{
	m_sprite = RES.getSprite("res/sprites/test/Kinght");
	//m_knight->setAnimation("idle");
	for (auto i : m_sprite->getAnimationsName())
		std::cout << i << std::endl;
	isDrawSize = true;
}

void Knight::Draw()
{
	RENDER.Render(m_sprite, m_position, m_size);
	IGameObject::Draw();
}

void Knight::Update(const float& delta_time)
{
	UPDATE.Update(m_sprite);
	IGameObject::Update(delta_time);
}

void Knight::KeyPress(const int& key)
{
}

void Knight::KeyUnpress(const int& key)
{
}
