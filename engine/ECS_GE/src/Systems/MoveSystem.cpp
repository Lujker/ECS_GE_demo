#include "MoveSystem.h"

#include "Move.h"
#include "Position.h"


MoveSystem& MoveSystem::Instance()
{
	static MoveSystem move_system;
	return move_system;
}

//! Calculate next position by velocity, acceleratin and time on milisecond
PositionComponent MoveSystem::Move(const PositionComponent& position, const MoveComponent& move, float delta_time)
{
	PositionComponent next_pos = position;
	const Vector2 add_posVec = (move.getVelocity() * (delta_time / 1000)) + ((move.getAcceleration() * pow((delta_time / 1000), 2)) / 2);
	next_pos = { position.getPosition() + FPoint{ add_posVec.x, add_posVec.y }, next_pos.getLayer(), next_pos.getRotation() };
	return next_pos;
}

//! Calculate next velocity by acceleratin and time on milisecond
MoveComponent MoveSystem::CalculateVelocity(const MoveComponent& move, float delta_time)
{
	MoveComponent nextMove = move;
	nextMove.SetVelocity(nextMove.getVelocity() + (nextMove.getAcceleration() * (delta_time / 1000)));
	return nextMove;
}

//! Calculate acceleration by gravity constant
MoveComponent MoveSystem::Gravity(const MoveComponent& move, float delta_time)
{
	MoveComponent nextMove = move;
	if (nextMove.getAcceleration().y >= m_gravity.y)
		nextMove.SetAcceleration((nextMove.getAcceleration() + m_gravity) / (delta_time / 1000));
	else
		nextMove.SetAcceleration({ nextMove.getAcceleration().x, m_gravity.y });
	return nextMove;
}
