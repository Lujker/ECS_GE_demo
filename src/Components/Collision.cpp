#include "Collision.h"

CollisionComponent::CollisionComponent(bool collision):
	collision(collision), m_form(std::make_shared<Form>())
{}

CollisionComponent::CollisionComponent(const FRect& rect, bool collision):
	collision(collision)
{
	m_form = std::make_shared<AABB>(rect);
}

CollisionComponent::CollisionComponent(const std::vector<FPoint>& points, bool collision):
	m_form(std::make_shared<Form>(points)), collision(collision)
{}

CollisionComponent::CollisionComponent(const CollisionComponent& collision):
	m_form(collision.m_form.get()), collision(collision.collision)
{
}

CollisionComponent::CollisionComponent(CollisionComponent&& collision) noexcept:
	m_form(std::move(collision.m_form)), collision(collision.collision)
{
}

CollisionComponent& CollisionComponent::operator=(const CollisionComponent& collision)
{
	m_form.reset(collision.m_form.get());
	this->collision = collision.collision;
	return (*this);
}

CollisionComponent& CollisionComponent::operator=(CollisionComponent&& collision) noexcept
{
	m_form = std::move(collision.m_form);
	this->collision = collision.collision;
	return (*this);
}

FRect CollisionComponent::getRect() const
{
	if (isRect())
		if (const auto rect = dynamic_cast<AABB*>(m_form.get()); rect)
			return rect->getRect();
	return FRect{};
}

void CollisionComponent::setRect(const FRect& rect)
{
	m_form = std::make_shared<AABB>(rect);
}

void CollisionComponent::setPolygons(const std::vector<FPoint>& points)
{
	m_form = std::make_shared<Form>(points);
}