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

class PositionComponent3 : IComponent
{
public:
	PositionComponent3() = default;
	PositionComponent3(const FPoint3 & position, const FPoint3& rotation);
	PositionComponent3(int x, int y, int z, int rotation_x = 0, int rotation_y = 0, int rotation_z = 0);
	PositionComponent3(double x, double y, double z, double rotation_x = 0., double rotation_y = 0., double rotation_z = 0.);
	PositionComponent3(const PositionComponent3& position_component) = default;
	~PositionComponent3() = default;

	PositionComponent3& operator=(const PositionComponent3& position_component);
	PositionComponent3 operator+(const FPoint3 & point) const;
	PositionComponent3 operator+(const PositionComponent3& position_component) const;
	PositionComponent3& operator+=(const FPoint3 & point);
	PositionComponent3& operator+=(const PositionComponent3& position_component);
	bool operator==(const PositionComponent3& position_component);
	bool operator!=(const PositionComponent3& position_component);

	void setPosition(const FPoint3& point) { m_position = point; }
	void setRotation(const FPoint3& rotation) { m_rotation = rotation; }

	FPoint3 getPosition() const { return  m_position; }
	FPoint3  getRotation() const { return  m_rotation; }

	FPoint3* getPositionPtr() { return &m_position; }
	FPoint3* getRotationPtr() { return &m_rotation; }

private:
	FPoint3 m_position;
	FPoint3 m_rotation;
};