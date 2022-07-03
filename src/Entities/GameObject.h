#pragma once
#include <string>

#include "Collision.h"
#include "DisplayString.h"
#include "InputManager.h"
#include "Move.h"
#include "Position.h"


class IGameObject: public InputListener
{
public:
	IGameObject(const std::string& name, const PositionComponent& default_position, 
		const CollisionComponent& default_size, const CollisionComponent& default_collision);
	virtual ~IGameObject() override = 0;
	virtual void Init();
	virtual void Draw();
	virtual void Update(const float& delta_time);

	[[nodiscard]] virtual PositionComponent		GetPosition() const { return m_position; }
	[[nodiscard]] virtual MoveComponent			GetMove() const { return m_move; }
	[[nodiscard]] virtual CollisionComponent	GetSize() const { return m_size; }
	[[nodiscard]] virtual CollisionComponent	GetCollision() const { return m_collision; }
	[[nodiscard]] virtual std::string			GetName() const { return m_name->getInitString(); }

	[[nodiscard]] virtual bool					IsCameraObject() const { return isCameraObject; }
	[[nodiscard]] virtual bool					IsGravityObject() const { return isGravityObject; }

	virtual void SetPosition(const PositionComponent& next_pos) { m_position = next_pos; }
	virtual void SetMove(const MoveComponent& next_move) { m_move = next_move; }
	virtual void SetSize(const CollisionComponent& next_size) { m_size = next_size; }
	virtual void SetCollision(const CollisionComponent& next_collision) { m_collision = next_collision; }

	virtual void MoveChange() {};
	virtual void PositionChange() {};
	virtual bool operator==(const IGameObject& object) const;
protected:
	std::shared_ptr<DisplayString>		m_name;
	PositionComponent					m_position;
	CollisionComponent					m_size;
	CollisionComponent					m_collision;
	MoveComponent						m_move;
	bool								isDrawSize = false;
	bool								isDrawCollision = false;
	bool								isDrawName = false;
	bool								isInit = false;

	//! remove to someone interface class
	bool								isCameraObject = false;
	bool								isGravityObject = false;
};

