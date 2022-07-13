#include "Collision.h"

#include <algorithm>

CollisionComponent::CollisionComponent(bool collision, float scale):
	m_form(nullptr), collision(collision), m_scale(scale)
{}

CollisionComponent::CollisionComponent(const FRect& rect, bool collision, float scale):
	collision(collision), m_scale(scale)
{
	m_form = std::make_shared<AABB>(rect);
}

CollisionComponent::CollisionComponent(float width, float hight, float x_offset, float y_offset, bool collision, float scale):
	CollisionComponent(FRect{ x_offset,y_offset, width, hight}, collision, scale)
{}

CollisionComponent::CollisionComponent(double width, double hight, double x_offset, double y_offset, bool collision,
	float scale) : CollisionComponent(FRect{ x_offset,y_offset, width, hight }, collision, scale)
{
}

CollisionComponent::CollisionComponent(const std::vector<FPoint>& points, bool collision, float scale):
	m_form(std::make_shared<Form>(points)), collision(collision), m_scale(scale)
{}

CollisionComponent::CollisionComponent(const CollisionComponent& collision):
	m_form(collision.m_form), collision(collision.collision), m_scale(collision.m_scale)
{
}

CollisionComponent::CollisionComponent(CollisionComponent&& collision) noexcept:
	m_form(std::move(collision.m_form)), collision(collision.collision), m_scale(collision.m_scale)
{
}

CollisionComponent& CollisionComponent::operator=(const CollisionComponent& collision)
{
	m_form = collision.m_form;
	this->collision = collision.collision;
	m_scale = collision.m_scale;
	return (*this);
}

CollisionComponent& CollisionComponent::operator=(CollisionComponent&& collision) noexcept
{
	m_form = std::move(collision.m_form);
	this->collision = collision.collision;
	m_scale = collision.m_scale;
	return (*this);
}

FRect CollisionComponent::getRect() const
{
	if (isRect())
		if (const auto rect = dynamic_cast<AABB*>(m_form.get()); rect)
			return rect->getRect();
	return FRect{};
}

FPoint CollisionComponent::getSize() const
{
	if (isValid())
	{
		const auto& itX = std::max_element( m_form->getPoints().begin(), m_form->getPoints().end(), [&](const auto& left, const auto& right)
			{
				return left.mX < right.mX;
			});
		const auto& itY = std::max_element( m_form->getPoints().begin(), m_form->getPoints().end(), [&](const auto& left, const auto& right)
			{
				return left.mY < right.mY;
			});
		return FPoint{ itX->mX - getXOffset(), itY->mY - getYOffset() };
	}
	return {};
}

double CollisionComponent::getWidth() const
{
	if (isValid())
	{
		return std::max_element(m_form->getPoints().begin(), m_form->getPoints().end(), [&](const auto& left, const auto& right)
			{
				return left.mX < right.mX;
			})->mX - getXOffset();
	}
	return 0.0;
}

double CollisionComponent::getHeight() const
{
	if (isValid())
	{
		return std::max_element(m_form->getPoints().begin(), m_form->getPoints().end(), [&](const auto& left, const auto& right)
			{
				return left.mY < right.mY;
			})->mY - getYOffset();
	}
	return 0.0;
}

double CollisionComponent::getXOffset() const
{
	if (isValid())
	{
		return m_form->getPoints().begin()->mX;
	}
	return 0.0;
}

double CollisionComponent::getYOffset() const
{
	if (isValid())
	{
		return m_form->getPoints().begin()->mY;
	}
	return 0.0;
}

FPoint CollisionComponent::getCenterPoint() const
{
	if (isValid())
	{
		return getSize() / 2;
	}
	return {};
}

float CollisionComponent::getScale() const
{
	return m_scale;
}

void CollisionComponent::setRect(const FRect& rect)
{
	m_form = std::make_shared<AABB>(rect);
}

void CollisionComponent::setPolygons(const std::vector<FPoint>& points)
{
	m_form = std::make_shared<Form>(points);
}