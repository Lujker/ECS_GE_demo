#pragma once
#include "IComponent.h"
#include "Shape.h"
#include "Rect.h"
#include "memory"

class CollisionComponent:public IComponent
{
public:
	CollisionComponent(bool collision = true, float scale = 1.f);
	CollisionComponent(const FRect& rect, bool collision = true, float scale = 1.f);
	CollisionComponent(float width, float hight, float x_offset = 0.f, float y_offset = 0, bool collision = true, float scale = 1.f);
	CollisionComponent(const std::vector<FPoint>& points, bool collision = true, float scale = 1.f);
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
	std::shared_ptr<Form> getForm() const { return m_form; }
	FRect getRect() const;
	FPoint getSize() const;
	float getWidth() const;
	float getHeight() const;
	float getXOffset() const;
	float getYOffset() const;
	FPoint getCenterPoint() const;
	float getScale() const;
	float* getScalePtr() { return &m_scale; }
	std::vector<FPoint> getPolygons() const noexcept
	{
		if (m_form)
			return m_form->getPoints();
		else return std::vector<FPoint>{};
	}
	bool hasCollision() { return collision; }
	void setRect(const FRect& rect);
	void setScale(float scale) { m_scale = scale; }
	void setPolygons(const std::vector<FPoint>& points);
	void setCollision(bool collision) { this->collision = collision;  }
private:
	std::shared_ptr<Form> m_form;
	bool collision = false;
	float m_scale = 1.f;
};