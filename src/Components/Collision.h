#pragma once
#include "IComponent.h"
#include "Shape.h"
#include "Rect.h"
#include "memory"

class CollisionComponent:public IComponent
{
public:
	CollisionComponent(bool collision = true);
	CollisionComponent(const FRect& rect, bool collision = true);
	CollisionComponent(float width, float hight, bool collision = true);
	CollisionComponent(const std::vector<FPoint>& points, bool collision = true);
	CollisionComponent(const CollisionComponent& collision);
	CollisionComponent(CollisionComponent&& collision) noexcept;
	~CollisionComponent() = default;

	CollisionComponent& operator=(const CollisionComponent& collision);
	CollisionComponent& operator=(CollisionComponent&& collision) noexcept;

	bool isValid() const noexcept
	{
		if (m_form)
			return m_form->getPoints().size() < 4 ? false : true;
		return false;
	}
	bool isRect() const noexcept
	{
		if (m_form)
			return m_form->getPoints().size() == 4 ? true : false;
		return false;
	}
	bool isPolygons() const noexcept { return isValid() && !isRect(); }
	FRect getRect() const;
	FPoint getSize() const;
	float getWidth() const;
	float getHeight() const;
	FPoint getCenterPoint() const;
	std::vector<FPoint> getPolygons() const noexcept
	{
		if (m_form)
			return m_form->getPoints();
		else return std::vector<FPoint>{};
	}
	bool hasCollision() { return collision; }

	void setRect(const FRect& rect);
	void setPolygons(const std::vector<FPoint>& points);
	void setCollision(bool collision) { this->collision = collision;  }
private:
	std::shared_ptr<Form> m_form;
	bool collision = false;
};