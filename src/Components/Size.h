#pragma once
#include "Rect.h"
#include "Vector.h"

class SizeComponent
{
public:
	SizeComponent();
	~SizeComponent() = default;
	SizeComponent(float width, float height);
	SizeComponent(const SizeComponent& size) = default;
	SizeComponent& operator=(const SizeComponent& size) = default;

	SizeComponent& operator+=(const SizeComponent& size);
	SizeComponent& operator+=(const FPoint& size);
	SizeComponent& operator+=(const Vector2& size);

	float getWidth() const { return m_width; }
	void setWidth(float width)  { m_width = width; }
	float getHeight() const { return m_height; }
	void setHeight(float height) { m_height = height; }
private:
	float m_width;
	float m_height;
};
