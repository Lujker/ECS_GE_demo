#pragma once
#include "IComponent.h"
#include "Point.h"

class PositionComponent : public IComponent
{
public:
	PositionComponent() = default;
	PositionComponent(const FPoint& position,
		float layer = 0,
		float rotation = 0);
	PositionComponent(int x, int y,
		float layer = 0,
		float rotation = 0);
	PositionComponent(double x, double y,
		float layer = 0,
		float rotation = 0);
	PositionComponent(const PositionComponent& position_component) = default;
	~PositionComponent() = default;

	PositionComponent& operator=(const PositionComponent& position_component) = default;
	PositionComponent operator+(const FPoint & point) const;
	PositionComponent operator+(const PositionComponent & position_component) const;
	PositionComponent& operator+=(const FPoint& point);
	PositionComponent& operator+=(const PositionComponent& position_component);
	bool operator==(const PositionComponent& position_component);
	bool operator!=(const PositionComponent& position_component);

	void setPosition(const FPoint& point) { m_position = point; }
	void setRotation(float rotation) { m_rotation = rotation; }
	void setLayer(float layer) { m_layer = layer; }

	FPoint getPosition() const { return  m_position; }
	float  getRotation() const { return  m_rotation; }
	float  getLayer() const { return m_layer; }

	FPoint* getPositionPtr() { return &m_position; }
	float* getLayerPtr() { return &m_layer; }
	float* getRotationPtr() { return &m_rotation; }

private:
	FPoint	m_position;
	float	m_layer;
	float	m_rotation;
};