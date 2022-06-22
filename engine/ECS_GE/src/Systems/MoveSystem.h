#pragma once

#include "ISystem.h"

#define MOVE MoveSystem::Instance()
class MoveComponent;
class PositionComponent;

class MoveSystem : public ISystem
{
	static MoveSystem& Instance();
	static void Move(PositionComponent& position, const MoveComponent& move, float delta_time);
private:
	MoveSystem() = default;
	~MoveSystem() = default;
	MoveSystem(const MoveSystem&) = delete;
	MoveSystem(MoveSystem&&) = delete;
	MoveSystem& operator=(const MoveSystem&) = delete;
	MoveSystem& operator=(MoveSystem&&) = delete;
};
