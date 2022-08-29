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

CollisionComponent3::CollisionComponent3(bool collision, float scale):
	m_form(nullptr), collision(collision), m_scale(scale)
{
}

CollisionComponent3::CollisionComponent3(float width, float hight, float depth, float x_offset, float y_offset, float z_offset, bool collision, float scale):
	collision(collision), m_scale(scale)
{
	std::vector<FPoint3> points{ 
		{x_offset, y_offset, z_offset}, {x_offset, y_offset, z_offset + depth}, {x_offset + width, y_offset, z_offset + depth}, {x_offset + width, y_offset, z_offset},
		{x_offset, y_offset + hight, z_offset}, {x_offset, y_offset + hight, z_offset + depth}, {x_offset + width, y_offset + hight, z_offset + depth}, {x_offset + width, y_offset + hight, z_offset}
	};
	m_form = std::make_shared<Form3>(points);
}

CollisionComponent3::CollisionComponent3(double width, double hight, double depth, double x_offset, double y_offset, double z_offset, bool collision, float scale):
	collision(collision), m_scale(scale)
{
	std::vector<FPoint3> points{
		{x_offset, y_offset, z_offset}, {x_offset, y_offset, z_offset + depth}, 
		{x_offset + width, y_offset, z_offset + depth}, {x_offset + width, y_offset, z_offset},
		{x_offset, y_offset + hight, z_offset}, {x_offset, y_offset + hight, z_offset + depth}, 
		{x_offset + width, y_offset + hight, z_offset + depth}, {x_offset + width, y_offset + hight, z_offset}
	};
	m_form = std::make_shared<Form3>(points);
}

CollisionComponent3::CollisionComponent3(const std::vector<FPoint3>& points, bool collision, float scale) :
	collision(collision), m_scale(scale)
{
	m_form = std::make_shared<Form3>(points);
}

CollisionComponent3::CollisionComponent3(const CollisionComponent3& collision):
	m_form(collision.m_form), collision(collision.collision), m_scale(collision.m_scale)
{
}

CollisionComponent3::CollisionComponent3(CollisionComponent3&& collision) noexcept
	: m_form(std::move(collision.m_form)), collision(std::move(collision.collision)), m_scale(std::move(collision.m_scale))
{
}

CollisionComponent3& CollisionComponent3::operator=(const CollisionComponent3& collision)
{
	m_form = collision.m_form;
	this->collision = collision.collision;
	m_scale = collision.m_scale;
	return (*this);
}

CollisionComponent3& CollisionComponent3::operator=(CollisionComponent3&& collision) noexcept
{
	m_form = std::move(collision.m_form);
	this->collision = std::move(collision.collision);
	m_scale = std::move(collision.m_scale);
	return (*this);
}

FPoint3 CollisionComponent3::getSize() const
{
	return FPoint3(getWidth(), getHeight(), getDepth());
}

double CollisionComponent3::getWidth() const
{
	return std::max_element(m_form->getPoints().begin(), m_form->getPoints().end(), [&](const auto& left, const auto& right)
		{
			return left.mX < right.mX;
		})->mX - getXOffset();
}

double CollisionComponent3::getHeight() const
{
	return std::max_element(m_form->getPoints().begin(), m_form->getPoints().end(), [&](const auto& left, const auto& right)
		{
			return left.mY < right.mY;
		})->mY - getYOffset();
}

double CollisionComponent3::getDepth() const
{
	return std::max_element(m_form->getPoints().begin(), m_form->getPoints().end(), [&](const auto& left, const auto& right)
		{
			return left.mZ < right.mZ;
		})->mZ - getZOffset();
}

double CollisionComponent3::getXOffset() const
{
	return std::min_element(m_form->getPoints().begin(), m_form->getPoints().end(), [&](const auto& left, const auto& right)
		{
			return left.mX < right.mX;
		})->mX;
}

double CollisionComponent3::getYOffset() const
{
	return std::min_element(m_form->getPoints().begin(), m_form->getPoints().end(), [&](const auto& left, const auto& right)
		{
			return left.mY < right.mY;
		})->mY;
}

double CollisionComponent3::getZOffset() const
{
	return std::min_element(m_form->getPoints().begin(), m_form->getPoints().end(), [&](const auto& left, const auto& right)
		{
			return left.mZ < right.mZ;
		})->mZ;
}

FPoint3 CollisionComponent3::getCenterPoint() const
{
	return FPoint3();
}

float CollisionComponent3::getScale() const
{
	return m_scale;
}

void CollisionComponent3::setPolygons(const std::vector<FPoint3>& points)
{
	m_form = std::make_shared<Form3>(points);
}
