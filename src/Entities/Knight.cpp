#include "Knight.h"
#include "DemoServiceLocator.h"
#include "CameraManager.h"
#include "LogSystem.h"
#include "MoveSystem.h"
#include "RenderSystem.h"
#include "ResourceManager.h"
#include "UpdateSystem.h"

Knight::Knight(const std::string& name, const PositionComponent& default_position, const CollisionComponent& default_size, const CollisionComponent& default_collision):
	IGameObject(name,default_position,default_size,default_collision)
{
}

Knight::~Knight()
{
	if (m_sprite)
		m_sprite->removeListener(this);
}

void Knight::Init()
{
	m_sprite = RES->getSprite("res/sprites/Humans/Kinght");
	if (m_sprite)
	{
		m_sprite->setAnimation("idle");
		for (auto i : m_sprite->getAnimationsName())
			std::cout << i << std::endl;
		m_sprite->addListener(this);
	}
	isDrawSize = true;
	isDrawName = true;
	isCameraObject = true;
	isGravityEnable = true;
	isPlayerControlledObject = true;
	IGameObject::Init();
}

void Knight::Draw()
{
	if (!isInit)
		return;
	RENDER->Render(m_sprite, m_position, m_size);
	IGameObject::Draw();
}

void Knight::Update(const float& delta_time)
{
	if (!isInit)
		return;
	UPDATE->Update(m_sprite);
	IGameObject::Update(delta_time);
}

void Knight::KeyPress(const int& key)
{
	if (!isInit || !isPlayerControlledObject)
		return;
	if (key == GLFW_KEY_A)
	{
		m_move.SetVelocity(m_move.getVelocity().x - 150.f, m_move.getVelocity().y);
	}
	if (key == GLFW_KEY_D)
	{
		m_move.SetVelocity(m_move.getVelocity().x + 150.f, m_move.getVelocity().y);
	}
	if (key == GLFW_KEY_W)
	{
		m_move.SetVelocity(m_move.getVelocity().x, m_move.getVelocity().y + 300.f);
	}
}

void Knight::KeyUnpress(const int& key)
{
	if (!isInit || !isPlayerControlledObject)
		return;
	if (key == GLFW_KEY_A)
	{
		m_move.SetVelocity(m_move.getVelocity().x + 150.f, m_move.getVelocity().y);
	}
	if (key == GLFW_KEY_D)
	{
		m_move.SetVelocity(m_move.getVelocity().x - 150.f, m_move.getVelocity().y); 
	}
	//if (key == GLFW_KEY_SPACE)
	//{
	//	std::queue<std::string> q;
	//	q.push("atack_combo");
	//	q.push("atack_combo2");
	//	q.push("idle");
	//	m_sprite->setAnimQueue(q);
	//}
}

void Knight::PositionChange()
{
}

void Knight::IsAnimationEnd(const std::string& anim)
{
}

void Knight::IsAnimationStart(const std::string& anim)
{
}


void Knight::MoveChange()
{
	if (m_move.getDirection() == MoveComponent::eDirection::eLeft || m_move.getDirection() == MoveComponent::eDirection::eUpLeft || m_move.getDirection() == MoveComponent::eDirection::eDownLeft)
	{
		if (!m_sprite->isHorizontalMirror())
			m_sprite->mirror(false, true);
	}
	else if (m_move.getDirection() == MoveComponent::eDirection::eRight || m_move.getDirection() == MoveComponent::eDirection::eUpRight || m_move.getDirection() == MoveComponent::eDirection::eDownRight)
	{
		if (m_sprite->isHorizontalMirror())
			m_sprite->mirror(false, false);
	}

	if (m_move.getDirection() == MoveComponent::eDirection::eRight || m_move.getDirection() == MoveComponent::eDirection::eLeft)
	{
		if (m_sprite->getCurrentAnimName() != "Run")
			m_sprite->setAnimation("Run");
	}
	else if (m_move.getDirection() == MoveComponent::eDirection::eUp || m_move.getDirection() == MoveComponent::eDirection::eUpRight || m_move.getDirection() == MoveComponent::eDirection::eUpLeft)
	{
		if (m_sprite->getCurrentAnimName() != "jump")
			m_sprite->setAnimation("jump");
	}
	else if (m_move.getDirection() == MoveComponent::eDirection::eDown || m_move.getDirection() == MoveComponent::eDirection::eDownRight || m_move.getDirection() == MoveComponent::eDirection::eDownLeft)
	{
		if (m_sprite->getCurrentAnimName() != "fall")
			m_sprite->setAnimation("fall");
	}	
	else
	{
		if (m_sprite->getCurrentAnimName() != "idle")
			m_sprite->setAnimation("idle");
	}
}
