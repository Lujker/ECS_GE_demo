#include "Move.h"

MoveComponent::MoveComponent(eDirection direction, const Vector2& velocity, const Vector2& acceleration):
	m_direction(direction), m_velocity(velocity), m_acceleration(acceleration)
{}

MoveComponent::MoveComponent(eDirection direction, float x_velocity, float y_velocity, float a_x, float a_y):
	m_direction(direction), m_velocity(x_velocity, y_velocity), m_acceleration({a_x, a_y })
{}

MoveComponent& MoveComponent::operator+=(const MoveComponent& move_component)
{
	m_direction = move_component.m_direction;
	m_velocity += move_component.m_velocity;
	m_acceleration += move_component.m_acceleration;
	return *this;
}

bool MoveComponent::operator==(const MoveComponent& move_component)
{
	return (m_direction==move_component.m_direction && m_velocity == move_component.m_velocity && m_acceleration == move_component.m_acceleration);
}

bool MoveComponent::operator!=(const MoveComponent& move_component)
{
	return !(*this==move_component);
}

void MoveComponent::UpdateDirection()
{
	if (m_velocity.x == 0.f && m_velocity.y == 0.f)
		m_direction = eDirection::eIdle;
	if (m_velocity.x > 0.f)
	{
		if (m_velocity.y > 0.f)
			m_direction = eDirection::eUpRight;
		else if (m_velocity.y == 0.f)
			m_direction = eDirection::eRight;
		else
			m_direction = eDirection::eDownRight;
	}
	else if (m_velocity.x < 0.f)
	{
		if (m_velocity.y > 0.f)
			m_direction = eDirection::eUpLeft;
		else if (m_velocity.y == 0.f)
			m_direction = eDirection::eLeft;
		else
			m_direction = eDirection::eDownLeft;
	}
	else
	{
		if (m_velocity.y > 0.f)
			m_direction = eDirection::eUp;
		else
			m_direction = eDirection::eDown;
	}
}
