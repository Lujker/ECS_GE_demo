#pragma once
#include "IComponent.h"
#include "Vector.h"

class MoveComponent : public IComponent
{
public:
	MoveComponent() = default;
	MoveComponent(const Vector2& direction, float velocity);
	MoveComponent(float x, float y, float velocity);
	MoveComponent(const MoveComponent& move_component) = default;
	~MoveComponent() = default;

	MoveComponent& operator=(const MoveComponent& move_component) = default;
	MoveComponent& operator+=(const Vector2& direction);
	MoveComponent& operator+=(const float velocity);
	MoveComponent& operator+=(const MoveComponent& move_component);

	void SetDirection(const Vector2& direction) { m_direction = direction; }
	void SetDirection(float x, float y) { m_direction = { x,y }; }
	void SetVelocity(float velocity) { m_velocity = velocity; }

	Vector2 getDirection() const { return m_direction; }
	float getVelocity() const { return m_velocity; }

private:
	Vector2 m_direction;
	float m_velocity;
};
