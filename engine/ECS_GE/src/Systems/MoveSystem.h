#pragma once

#include "ISystem.h"
#include "Vector.h"

#define MOVE MoveSystem::Instance()
class MoveComponent;
class PositionComponent;

class MoveSystem : public ISystem
{
public:
	static MoveSystem& Instance();
	PositionComponent Move(const PositionComponent& position, const MoveComponent& move, float delta_time);
	MoveComponent CalculateVelocity(const MoveComponent& move, float delta_time);
	MoveComponent Gravity(const MoveComponent& move, float delta_time);
private:
	Vector2 m_gravity = { 0.f, -9.80665f };
	MoveSystem() = default;
	~MoveSystem() = default;
	MoveSystem(const MoveSystem&) = delete;
	MoveSystem(MoveSystem&&) = delete;
	MoveSystem& operator=(const MoveSystem&) = delete;
	MoveSystem& operator=(MoveSystem&&) = delete;
};
