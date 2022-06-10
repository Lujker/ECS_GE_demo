#pragma once

#include <vector>
#include "glm/vec2.hpp"
#include "Rect.h"
#include "Vector.h"

class Form
{
public:
	Form() = default;
	virtual ~Form() = default;
	Form(const Form& figure);
	Form(std::initializer_list<FPoint> in_list);
	Form(const std::vector<FPoint>&);

	Form& operator=(const Form& figure);
	Form& operator=(std::initializer_list<FPoint> in_list);

	virtual bool intersect2D(const Form& figure) const;
	virtual bool include2D(const Form& figure) const;
	virtual bool include2D(const FPoint& point) const;

	//!TODO
	static bool intersect2D(const Form& left_figure, const Form& right_figure);
	static bool include2D(const Form& left_figure, const Form& right_figure);
	static bool include2D(const Form& figure, const FPoint& point);

	void setPoints(const std::vector<FPoint>& points);
	void setPoints(std::vector<FPoint>&& points);
	const std::vector<FPoint>& getPoints();
	Form operator+(const Vector2& point) const;
protected:
	std::vector<FPoint> m_points;
};

class AABB : public Form
{
public:
	AABB() = default;
	~AABB() = default;
	AABB(const AABB& figure);
	AABB(const FRect& rect);
	AABB(std::initializer_list<FPoint> in_list);

	AABB& operator=(const AABB& figure);
	AABB& operator=(const FRect& rect);
	AABB& operator=(std::initializer_list<FPoint> in_list);

	AABB operator+(const Vector2& point) const;
	AABB operator-(const Vector2& point) const;

	bool intersect2D(const AABB& figure) const;
	static bool intersect2D(const AABB& left_figure, const AABB& right_figure);
	void setRect(const FRect& rect);
	const FRect getRect() const;
};