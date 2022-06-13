#include "MoveSystem.h"

#include "Move.h"
#include "Position.h"

MoveSystem& MoveSystem::Instance()
{
	static MoveSystem move_system;
	return move_system;
}

void MoveSystem::Move(PositionComponent& position, const MoveComponent& move, float delta_time)
{
	const auto new_posVec = move.getDirection() * move.getVelocity() * delta_time;
	position = position.getPosition() * FPoint{ new_posVec.x, new_posVec.y };
}
