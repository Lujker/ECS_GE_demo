#pragma once
#include "Point.h"

class PositionComponent
{
public:
	PositionComponent() = default;
	PositionComponent(const FPoint& position,
		float layer = 0,
		float rotation = 0);
	PositionComponent(const PositionComponent& position_component) = default;
	~PositionComponent() = default;

	PositionComponent& operator=(const PositionComponent& position_component) = default;
	PositionComponent& operator+=(const FPoint& point);
	PositionComponent& operator+=(const PositionComponent& position_component);

	void setPosition(const FPoint& point) { m_position = point; }
	void setRotation(float rotation) { m_rotation = rotation; }
	void setLayer(float layer) { m_layer = layer; }

	FPoint getPosition() const { return  m_position; }
	float  getRotation() const { return  m_rotation; }
	float  getLayer() const { return m_layer; }
private:
	FPoint	m_position;
	float	m_layer;
	float	m_rotation;
};