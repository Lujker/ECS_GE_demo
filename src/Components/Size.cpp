#include "Size.h"

SizeComponent::SizeComponent():
	m_width(0), m_height(0)
{}

SizeComponent::SizeComponent(float width, float height) :
	m_width(width), m_height(height)
{}

SizeComponent& SizeComponent::operator+=(const SizeComponent& size)
{
	m_width += size.m_width;
	m_height += size.m_height;
	return *this;
}

SizeComponent& SizeComponent::operator+=(const FPoint& size)
{
	m_width += size.mX;
	m_height += size.mY;
	return *this;
}

SizeComponent& SizeComponent::operator+=(const Vector2& size)
{
	m_width += size.x;
	m_height += size.y;
	return *this;
}
