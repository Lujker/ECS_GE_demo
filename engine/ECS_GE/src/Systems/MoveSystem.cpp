#include "MoveSystem.h"

#include "Move.h"
#include "Position.h"


MoveSystem& MoveSystem::Instance()
{
	static MoveSystem move_system;
	return move_system;
}

PositionComponent MoveSystem::Move(const PositionComponent& position, const MoveComponent& move, float delta_time)
{
	PositionComponent next_pos = position;
	const MoveComponent next_move = CalculateVelocity(move, delta_time);
	const Vector2 add_posVec = next_move.getVelocity() * (delta_time / 1000);
	next_pos = { position.getPosition() + FPoint{ add_posVec.x, add_posVec.y }, next_pos.getLayer(), next_pos.getRotation() };
	return next_pos;
}

MoveComponent MoveSystem::CalculateVelocity(const MoveComponent& move, float delta_time)
{
	MoveComponent nextMove = move;
	nextMove.SetVelocity(nextMove.getVelocity() + (nextMove.getAcceleration() * (delta_time / 1000)));
	return nextMove;
}

MoveComponent MoveSystem::Gravity(const MoveComponent& move, float delta_time)
{
	MoveComponent nextMove = move;
	nextMove.SetAcceleration(nextMove.getAcceleration() + m_gravity / (delta_time / 1000));
	return nextMove;
}
