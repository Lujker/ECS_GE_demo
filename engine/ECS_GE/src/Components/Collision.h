#pragma once
#include "IComponent.h"
#include "Shape.h"
#include "Rect.h"
#include "memory"

class CollisionComponent : public IComponent
{
public:
	CollisionComponent(bool collision = true, float scale = 1.f);
	CollisionComponent(const FRect& rect, bool collision = true, float scale = 1.f);
	CollisionComponent(float width, float hight, float x_offset = 0.f, float y_offset = 0.f, bool collision = true, float scale = 1.f);
	CollisionComponent(double width, double hight, double x_offset = 0., double y_offset = 0., bool collision = true, float scale = 1.f);
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
	double getWidth() const;
	double getHeight() const;
	double getXOffset() const;
	double getYOffset() const;
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

class CollisionComponent3 : public IComponent
{
public:
	CollisionComponent3(bool collision = true, float scale = 1.f);
	CollisionComponent3(float width, float hight, float depth, float x_offset = 0.f, float y_offset = 0.f, float z_offset = 0.f, bool collision = true, float scale = 1.f);
	CollisionComponent3(double width, double hight, double depth, double x_offset = 0., double y_offset = 0., double z_offset = 0., bool collision = true, float scale = 1.f);
	CollisionComponent3(const std::vector<FPoint3>& points, bool collision = true, float scale = 1.f);
	CollisionComponent3(const CollisionComponent3& collision);
	CollisionComponent3(CollisionComponent3&& collision) noexcept;
	~CollisionComponent3() = default;

	CollisionComponent3& operator=(const CollisionComponent3& collision);
	CollisionComponent3& operator=(CollisionComponent3&& collision) noexcept;

	bool isValid() const noexcept
	{
		if (m_form)
			return m_form->getPoints().size() < 8 ? false : true;
		return false;
	}
	std::shared_ptr<Form3> getForm() const { return m_form; }
	FPoint3		getSize() const;
	double		getWidth() const;
	double		getHeight() const;
	double		getDepth() const;
	double		getXOffset() const;
	double		getYOffset() const;
	double		getZOffset() const;
	FPoint3		getCenterPoint() const;
	float		getScale() const;

	float* getScalePtr() { return &m_scale; }
	std::vector<FPoint3> getPolygons() const noexcept
	{
		if (m_form)
			return m_form->getPoints();
		else return std::vector<FPoint3>{};
	}
	bool hasCollision() { return collision; }
	void setScale(float scale) { m_scale = scale; }
	void setPolygons(const std::vector<FPoint3>& points);
	void setCollision(bool collision) { this->collision = collision; }
private:
	std::shared_ptr<Form3> m_form;
	bool collision = false;
	float m_scale = 1.f;
};