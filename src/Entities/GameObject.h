#pragma once
#include <string>

#include "Collision.h"
#include "DisplayString.h"
#include "InputManager.h"
#include "Move.h"
#include "Position.h"

class IObject
{
public:
	IObject(const PositionComponent& default_position, const CollisionComponent& default_size);
	virtual ~IObject() = 0;

	virtual void Init();
	virtual void Draw();
	virtual void Update(const float& delta_time);
	[[nodiscard]] virtual CollisionComponent	GetSize() const { return m_size; }
	[[nodiscard]] virtual PositionComponent		GetPosition() const { return m_position; }
	virtual void SetSize(const CollisionComponent& next_size) { m_size = next_size; }
	virtual void SetPosition(const PositionComponent& next_pos) { m_position = next_pos; }
protected:
	PositionComponent					m_position;
	CollisionComponent					m_size;
	bool								isDrawSize = false;
	bool								isDrawCollision = false;
	bool								isDrawName = false;
	bool								isInit = false;
};

struct IGravityObject
{
	virtual ~IGravityObject();
	virtual bool gravityEnable() { return isGravityEnable; }
	virtual void setGravityEnable(bool enable) { isGravityEnable = enable; }
protected:
	bool isGravityEnable = false;
};

struct ICameraObject
{
	virtual ~ICameraObject();
	virtual bool cameraObject() { return isCameraObject; }
	virtual void setIsCamera(bool enable) { isCameraObject = enable; }
protected:
	bool isCameraObject = false;
};

class IGameObject: public IObject, public IGravityObject, public ICameraObject, public InputListener
{
public:
	IGameObject(const std::string& name, const PositionComponent& default_position, 
		const CollisionComponent& default_size, const CollisionComponent& default_collision);
	virtual ~IGameObject() override = 0;
	virtual bool operator==(const IGameObject& object) const;
	virtual void Draw() override;

	[[nodiscard]] virtual MoveComponent			GetMove() const { return m_move; }
	[[nodiscard]] virtual CollisionComponent	GetCollision() const { return m_collision; }
	[[nodiscard]] virtual std::string			GetName() const { return m_name->getInitString(); }

	virtual void SetMove(const MoveComponent& next_move) { m_move = next_move; }
	virtual void SetCollision(const CollisionComponent& next_collision) { m_collision = next_collision; }

	//!callback's
	virtual void MoveChange() {}
	virtual void PositionChange() {}
protected:
	std::shared_ptr<DisplayString>		m_name;
	CollisionComponent					m_collision;
	MoveComponent						m_move;
};

class IInterfaceObject : public IObject, public InputListener
{
public:
	IInterfaceObject(unsigned id, const PositionComponent& default_position,
		const CollisionComponent& default_size, const CollisionComponent& default_collision);
	virtual ~IInterfaceObject() override = 0;

	virtual void Draw() override;
	[[nodiscard]] virtual CollisionComponent	GetCollision() const { return m_collision; }
	[[nodiscard]] virtual unsigned				GetId() const { return m_id; }

	virtual void SetCollision(const CollisionComponent& next_collision) { m_collision = next_collision; }
protected:
	unsigned							m_id;
	CollisionComponent					m_collision;
};