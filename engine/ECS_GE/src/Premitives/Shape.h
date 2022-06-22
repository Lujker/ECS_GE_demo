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
	Form operator+(const Vector2& point) const;

	Vector2 getCenter();
	void BuildEdges();
	//!TODO
	static bool intersect2D(const Form& left_figure, const FPoint& left_position, const Form& right_figure, const FPoint& right_position);
	static bool include2D(const Form& left_figure, const FPoint& left_position, const Form& right_figure, const FPoint& right_position);
	static bool include2D(const Form& figure, const FPoint& position, const FPoint& point);

	void setPoints(const std::vector<FPoint>& points);
	void setPoints(std::vector<FPoint>&& points);
	const std::vector<FPoint>& getPoints() const;
	std::vector<FPoint> getEdges() const;
protected:
	std::vector<FPoint> m_points;
	std::vector<FPoint> m_edges;
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
	
	static bool intersect2D(const AABB& left_figure, const FPoint& left_position, const AABB& right_figure, const FPoint& right_position);
	void setRect(const FRect& rect);
	const FRect getRect() const;
};