#pragma once
#include <string>

#include "Collision.h"
#include "DisplayString.h"
#include "InputManager.h"
#include "Position.h"


class IGameObject: public InputListener
{
public:
	IGameObject(const std::string& name, const PositionComponent& default_position, const CollisionComponent& default_size, const CollisionComponent& default_collision);
	virtual ~IGameObject() override = 0;
	virtual void Init();
	virtual void Draw();
	virtual void Update(const float& delta_time);
protected:
	std::shared_ptr<DisplayString>		m_name;
	PositionComponent					m_position;
	CollisionComponent					m_size;
	CollisionComponent					m_collision;
	bool								isDrawSize = false;
	bool								isDrawCollision = false;
	bool								isDrawName = false;
};