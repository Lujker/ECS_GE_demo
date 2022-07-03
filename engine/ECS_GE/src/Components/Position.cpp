#include "Position.h"
#include <algorithm>

PositionComponent::PositionComponent(const FPoint& position, float layer, float rotation):
	m_position(position), m_layer(layer), m_rotation(rotation)
{}

PositionComponent::PositionComponent(int x, int y, float layer, float rotation):
	m_position({static_cast<double>(x),static_cast<double>(y)}), m_layer(layer), m_rotation(rotation)
{
}

PositionComponent::PositionComponent(double x, double y, float layer, float rotation):
	m_position( x, y ), m_layer(layer), m_rotation(rotation)
{
}

PositionComponent PositionComponent::operator+(const FPoint& point) const
{
	auto new_pos = *this;
	new_pos += point;
	return new_pos;
}

PositionComponent PositionComponent::operator+(const PositionComponent& position_component) const
{
	auto new_pos = *this;
	new_pos += position_component;
	return new_pos;
}

PositionComponent& PositionComponent::operator+=(const FPoint& point)
{
	m_position += point;
	return *this;
}

PositionComponent& PositionComponent::operator+=(const PositionComponent& position_component)
{
	m_position += position_component.m_position;
	m_layer += position_component.m_layer;
	m_rotation += position_component.m_rotation;
	return *this;
}

bool PositionComponent::operator==(const PositionComponent& position_component)
{
	return (m_position == position_component.m_position && m_rotation == position_component.m_rotation && m_layer == position_component.m_layer);
}

bool PositionComponent::operator!=(const PositionComponent& position_component)
{
	return !(*this==position_component);
}
