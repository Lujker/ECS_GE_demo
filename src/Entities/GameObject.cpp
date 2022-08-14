#include "GameObject.h"
#include "DemoServiceLocator.h"
#include "RenderSystem.h"


IObject::IObject(const PositionComponent& default_position, const CollisionComponent& default_size) :
	m_position(default_position),
	m_size(default_size)
{}

IObject::~IObject()
{
}

void IObject::Init()
{
	isInit = true;
}

void IObject::Draw()
{
	if (!isInit)
		return;
	if (isDrawSize)
		RENDER->Render(m_size, m_position);
}

void IObject::Update(const float& delta_time)
{
}

IGravityObject::~IGravityObject()
{
}

ICameraObject::~ICameraObject()
{
}

IPlayerControlledObject::~IPlayerControlledObject()
{
}

IGameObject::IGameObject(const std::string& name, 
                         const PositionComponent& default_position, 
                         const CollisionComponent& default_size, 
                         const CollisionComponent& default_collision) :
	IObject(default_position, default_size), m_name(std::make_shared<DisplayString>(name)),  m_collision(default_collision)
{}

IGameObject::~IGameObject()
{}

bool IGameObject::operator==(const IGameObject& object) const
{
	return GetName() == object.GetName();
}

void IGameObject::Draw()
{
	IObject::Draw();
	if (isDrawCollision)
		RENDER->Render(m_collision, m_position);
	if (isDrawName)
	{
		RENDER->Render(m_name, m_position + FPoint{ 0.f, m_collision.getHeight() }, 0.5);
		auto r = m_name->getRect(0.5f);
		r.mX += m_position.getPosition().mX;
		r.mY += m_position.getPosition().mY + m_collision.getHeight();
		RENDER->Render(r);
	}
}

IInterfaceObject::IInterfaceObject(unsigned id, const PositionComponent& default_position, const CollisionComponent& default_size, const CollisionComponent& default_collision):
	IObject(default_position, default_size), m_id(id), m_collision(default_collision)
{
}

IInterfaceObject::~IInterfaceObject()
{}

void IInterfaceObject::Draw()
{
	IObject::Draw();
	if (isDrawCollision)
		RENDER->Render(m_collision, m_position);
}