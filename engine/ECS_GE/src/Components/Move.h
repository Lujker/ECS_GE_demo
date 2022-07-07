#pragma once
#include "IComponent.h"
#include "Vector.h"

class MoveComponent : public IComponent
{
public:
	enum class eDirection : unsigned
	{
		eIdle = 0,
		eLeft = 1,
		eRight = 2,
		eUp = 3,
		eUpLeft = 4,
		eUpRight = 5,
		eDown = 6,
		eDownLeft = 7,
		eDownRight = 8,
	};

	MoveComponent();
	MoveComponent(eDirection direction, const Vector2& velocity, const Vector2& acceleration = {0.f,0.f});
	MoveComponent(eDirection direction, float x_velocity, float y_velocity, float a_x = 0.f, float a_y = 0.f);
	MoveComponent(const MoveComponent& move_component);
	~MoveComponent() = default;

	MoveComponent& operator=(const MoveComponent& move_component);
	MoveComponent& operator+=(const MoveComponent& move_component);
	bool operator==(const MoveComponent& move_component);
	bool operator!=(const MoveComponent& move_component);

	void SetDirection(eDirection direction) { m_direction = direction; }
	void SetVelocity(const Vector2& velocity) { m_velocity = velocity; }
	void UpdateDirection();
	void SetVelocity(float x, float y) { m_velocity = {x, y}; }
	void SetAcceleration(const Vector2& acceleration) { m_acceleration = acceleration; }

	eDirection getDirection() const { return m_direction; }
	Vector2 getAcceleration() const { return m_acceleration; }
	Vector2 getVelocity() const { return m_velocity; }

private:
	eDirection m_direction;
	Vector2 m_velocity;
	Vector2 m_acceleration;
};
