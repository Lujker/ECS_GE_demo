#include "GameObject.h"

#include "RenderSystem.h"

IGameObject::IGameObject(const std::string& name, 
                         const PositionComponent& default_position, 
                         const CollisionComponent& default_size, 
                         const CollisionComponent& default_collision) :
	m_name(std::make_shared<DisplayString>(name)), m_position(default_position),
	m_size(default_size), m_collision(default_collision)
{
}

IGameObject::~IGameObject()
{}

void IGameObject::Init()
{
}

void IGameObject::Draw()
{
	if (isDrawCollision)
		RENDER.Render(m_collision, m_position);
	if (isDrawSize)
		RENDER.Render(m_size, m_position);
	if (isDrawName)
	{
		RENDER.Render(m_name, m_position + FPoint{ 0.f, m_size.getHeight() }, 0.5, {0.4f,0.5,0.7f,1});
	}
}

void IGameObject::Update(const float& delta_time)
{
}
