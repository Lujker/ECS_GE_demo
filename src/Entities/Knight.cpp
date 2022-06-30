#include "Knight.h"

#include "MoveSystem.h"
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
	m_sprite->setAnimation("idle");
	for (auto i : m_sprite->getAnimationsName())
		std::cout << i << std::endl;
	isDrawSize = true;
	isDrawName = true;
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
	//m_move = MOVE.Gravity(m_move);
	m_position = MOVE.Move(m_position, m_move, delta_time);
}

void Knight::KeyPress(const int& key)
{
	if (key == GLFW_KEY_LEFT)
	{
		m_move.SetVelocity(m_move.getVelocity().x - 100.f, m_move.getVelocity().y);
	}
	if (key == GLFW_KEY_RIGHT)
	{
		m_move.SetVelocity(m_move.getVelocity().x + 100.f, m_move.getVelocity().y);
	}
	if (key == GLFW_KEY_UP)
	{
		m_move.SetVelocity(m_move.getVelocity().x, m_move.getVelocity().y + 100.f);
	}
	if (key == GLFW_KEY_DOWN)
	{
		m_move.SetVelocity(m_move.getVelocity().x, m_move.getVelocity().y - 100.f);
	}

	if (m_move.getVelocity().x < 0)
	{
		m_sprite->mirror(false, true);
		m_sprite->setAnimation("Run");
	}
	else if (m_move.getVelocity().x > 0)
	{
		m_sprite->mirror(false, false);
		m_sprite->setAnimation("Run");
	}
	else
	{
		m_sprite->setAnimation("idle");
	}
}

void Knight::KeyUnpress(const int& key)
{
	if (key == GLFW_KEY_LEFT)
	{
		m_move.SetVelocity(m_move.getVelocity().x + 100.f, m_move.getVelocity().y);
	}
	if (key == GLFW_KEY_RIGHT)
	{
		m_move.SetVelocity(m_move.getVelocity().x - 100.f, m_move.getVelocity().y);
	}
	if (key == GLFW_KEY_UP)
	{
		m_move.SetVelocity(m_move.getVelocity().x, m_move.getVelocity().y - 100.f);
	}
	if (key == GLFW_KEY_DOWN)
	{
		m_move.SetVelocity(m_move.getVelocity().x, m_move.getVelocity().y + 100.f);
	}

	if (m_move.getVelocity().x < 0)
	{
		m_sprite->mirror(false, true);
		m_sprite->setAnimation("Run");
	}
	else if (m_move.getVelocity().x > 0)
	{
		m_sprite->mirror(false, false);
		m_sprite->setAnimation("Run");
	}
	else
	{
		m_sprite->setAnimation("idle");
	}
}
