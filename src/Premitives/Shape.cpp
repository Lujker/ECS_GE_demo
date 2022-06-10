#include "Shape.h"

	Form::Form(const Form& figure) :
		m_points(figure.m_points)
	{
	}
	Form::Form(std::initializer_list<FPoint> in_list)
	{
		for (const auto& it : in_list)
			m_points.emplace_back(it);
	}

	Form::Form(const std::vector<FPoint>& points):
		m_points(points)
	{
	}

	Form& Form::operator=(const Form& figure)
	{
		m_points = figure.m_points;
		return *this;
	}

	Form& Form::operator=(std::initializer_list<FPoint> in_list)
	{
		m_points = in_list;
		return *this;
	}

	bool Form::intersect2D(const Form& figure) const
	{
		return intersect2D(*this, figure);
	}

	bool Form::include2D(const Form& figure) const
	{
		return include2D(*this, figure);
	}

	bool Form::include2D(const FPoint& point) const
	{
		return include2D(*this, point);
	}
	//!TODO Left figure intersect right figure?
	bool Form::intersect2D(const Form& left_figure, const Form& right_figure)
	{
		return false;
	}
	//!TODO Left figure has plase in right figure?
	bool Form::include2D(const Form& left_figure, const Form& right_figure)
	{
		return false;
	}

	bool Form::include2D(const Form& figure, const FPoint& point)
	{
		bool result = false;
		size_t j = figure.m_points.size() - 1;
		for (size_t i = 0; i < figure.m_points.size(); i++) {
			if ((figure.m_points[i].mY < point.mY && figure.m_points[j].mY >= point.mY || figure.m_points[j].mY < point.mY && figure.m_points[i].mY >= point.mY) &&
				(figure.m_points[i].mX + (point.mY - figure.m_points[i].mY) / (figure.m_points[j].mY - figure.m_points[i].mY) * (figure.m_points[j].mX - figure.m_points[i].mX) < point.mX))
				result = !result;
			j = i;
		}
		return result;
	}

	void Form::setPoints(const std::vector<FPoint>& points)
	{
		m_points = points;
	}

	void Form::setPoints(std::vector<FPoint>&& points)
	{
		m_points = points;
	}

	const std::vector<FPoint>& Form::getPoints()
	{
		return m_points;
	}

	Form Form::operator+(const Vector2& point) const
	{
		Form next(*this);
		for (auto it : next.m_points)
			it += {point.x, point.y};
		return next;
	}
	AABB::AABB(const AABB& figure) :
		Form(figure)
	{
	}
	AABB::AABB(const FRect& rect) :
		Form({ {rect.mX, rect.mY}, {rect.mX, rect.mY + rect.mHeight},
								{rect.mX + rect.mWidth, rect.mY + rect.mHeight}, {rect.mX + rect.mWidth, rect.mY} })
	{
	}
	AABB::AABB(std::initializer_list<FPoint> in_list)
	{
		m_points.reserve(4);
		for (auto it : in_list)
			m_points.push_back(std::move(it));
	}
	AABB& AABB::operator=(const AABB& figure)
	{
		m_points = figure.m_points;
		return *this;
	}
	AABB& AABB::operator=(const FRect& rect)
	{
		setRect(rect);
		return *this;
	}
	AABB& AABB::operator=(std::initializer_list<FPoint> in_list)
	{
		m_points = in_list;
		return *this;
	}
	AABB AABB::operator+(const Vector2& point) const
	{
		AABB next(*this);
		for (auto it : next.m_points)
			it += {point.x, point.y};
		return next;
	}
	AABB AABB::operator-(const Vector2& point) const
	{
		AABB next(*this);
		for (auto it : next.m_points)
			it -= {point.x, point.y};
		return next;
	}
	bool AABB::intersect2D(const AABB& figure) const
	{
		return intersect2D(*this, figure);
	}
	bool AABB::intersect2D(const AABB& left_figure, const AABB& right_figure)
	{
		return left_figure.getRect().Intersects(right_figure.getRect());
	}
	void AABB::setRect(const FRect& rect)
	{
		m_points.clear();
		m_points.reserve(4);
		m_points.emplace_back(FPoint{ rect.mX, rect.mY });
		m_points.emplace_back(FPoint{ rect.mX, rect.mY + rect.mHeight });
		m_points.emplace_back(FPoint{ rect.mX + rect.mWidth, rect.mY + rect.mHeight });
		m_points.emplace_back(FPoint{ rect.mX + rect.mWidth, rect.mY });
	}
	const FRect AABB::getRect() const
	{
		FRect rect;
		if (!m_points.empty() && m_points.size() == 4)
			rect = { m_points.at(0).mX, m_points.at(0).mY,
				m_points.at(2).mX - m_points.at(0).mX,
				m_points.at(2).mY - m_points.at(0).mY };
		return rect;
	}
