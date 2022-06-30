#pragma once

#include "ISystem.h"
#include "Vector.h"

#define MOVE MoveSystem::Instance()
class MoveComponent;
class PositionComponent;

class MoveSystem : public ISystem
{
	static MoveSystem& Instance();
	static PositionComponent Move(const PositionComponent& position, const MoveComponent& move, float delta_time);
	static MoveComponent CalculateVelocity(const MoveComponent& move, float delta_time, bool withGravity = true);
private:
	static Vector2 m_gravity;
	MoveSystem() = default;
	~MoveSystem() = default;
	MoveSystem(const MoveSystem&) = delete;
	MoveSystem(MoveSystem&&) = delete;
	MoveSystem& operator=(const MoveSystem&) = delete;
	MoveSystem& operator=(MoveSystem&&) = delete;
};
