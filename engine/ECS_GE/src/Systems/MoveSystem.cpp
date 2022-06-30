#include "MoveSystem.h"

#include "Move.h"
#include "Position.h"

Vector2  MoveSystem::m_gravity = { 0.f, -9.80665f };

MoveSystem& MoveSystem::Instance()
{
	static MoveSystem move_system;
	return move_system;
}

PositionComponent MoveSystem::Move(const PositionComponent& position, const MoveComponent& move, float delta_time)
{
	PositionComponent next_pos = position;
	const MoveComponent next_move = CalculateVelocity(move, delta_time);
	const Vector2 add_posVec = next_move.getVelocity() * delta_time;
	next_pos = position.getPosition() + FPoint{ add_posVec.x, add_posVec.y };
	return next_pos;
}

MoveComponent MoveSystem::CalculateVelocity(const MoveComponent& move, float delta_time, bool withGravity)
{
	MoveComponent nextMove = move;
	nextMove.SetVelocity(nextMove.getVelocity() + (nextMove.getAcceleration() * delta_time));
	if (withGravity)
		nextMove.SetAcceleration(nextMove.getAcceleration() + m_gravity);
	return nextMove;
}
