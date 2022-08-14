#pragma once

#include "ISystem.h"
#include "Vector.h"


class MoveComponent;
class PositionComponent;

class MoveSystem : public ISystem
{
public:
	friend class ServiceLocator;
	PositionComponent Move(const PositionComponent& position, const MoveComponent& move, float delta_time);
	MoveComponent CalculateVelocity(const MoveComponent& move, float delta_time);
	MoveComponent Gravity(const MoveComponent& move, float delta_time);

	MoveSystem(const MoveSystem&) = delete;
	MoveSystem(MoveSystem&&) = delete;
	MoveSystem& operator=(const MoveSystem&) = delete;
	MoveSystem& operator=(MoveSystem&&) = delete;
private:
	Vector2 m_gravity = { 0.f, -9.80665f };
	MoveSystem() = default;
	~MoveSystem() override = default;
};
