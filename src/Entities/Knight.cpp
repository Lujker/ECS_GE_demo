#include "Knight.h"

#include "CameraManager.h"
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
	IGameObject::Init();
}

void Knight::Draw()
{
	if (!isInit)
		return;
	RENDER.Render(m_sprite, m_position, m_size);
	IGameObject::Draw();
}

void Knight::Update(const float& delta_time)
{
	if (!isInit)
		return;
	UPDATE.Update(m_sprite);
	IGameObject::Update(delta_time);
	//auto next_pos = MOVE.Move(m_position, m_move, delta_time);

	//CAMERA.Move(next_pos.getPosition().mX - m_position.getPosition().mX, next_pos.getPosition().mY - m_position.getPosition().mY);
	//m_position = next_pos;
	//if (m_position.getPosition().mY > 0)
	//	m_move = MOVE.Gravity(m_move, delta_time);
	//else
	//{
	//	m_move.SetAcceleration({ 0.f, 0.f });
	//	m_move.SetVelocity(m_move.getVelocity().x, 0);
	//	m_position.setPosition({ m_position.getPosition().mX, 0 });
	//}
}

void Knight::KeyPress(const int& key)
{
	if (!isInit)
		return;
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

	CheckDirection();
}

void Knight::KeyUnpress(const int& key)
{
	if (!isInit)
		return;
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

	CheckDirection();
}

void Knight::CheckDirection()
{
	if (m_move.getVelocity().x < 0)
	{
		m_sprite->mirror(false, true);
		if(m_sprite->getCurrentAnimName() != "Run")
			m_sprite->setAnimation("Run");
	}
	else if (m_move.getVelocity().x > 0)
	{
		m_sprite->mirror(false, false);
		if (m_sprite->getCurrentAnimName() != "Run")
		m_sprite->setAnimation("Run");
	}
	else
	{
		if (m_sprite->getCurrentAnimName() != "idle")
			m_sprite->setAnimation("idle");
	}
}
