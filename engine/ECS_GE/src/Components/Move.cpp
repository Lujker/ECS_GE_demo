#include "Move.h"

MoveComponent::MoveComponent(const Vector2& direction, float velocity):
	m_direction(direction), m_velocity(velocity)
{}

MoveComponent::MoveComponent(float x, float y, float velocity):
	m_direction(x,y), m_velocity(velocity)
{}

MoveComponent& MoveComponent::operator+=(const Vector2& direction)
{
	m_direction += direction;
	return *this;
}

MoveComponent& MoveComponent::operator+=(const float velocity)
{
	m_velocity += velocity;
	return *this;
}

MoveComponent& MoveComponent::operator+=(const MoveComponent& move_component)
{
	m_direction += move_component.m_direction;
	m_velocity += move_component.m_velocity;
	return *this;
}
