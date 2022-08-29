#include "Shape.h"


// Calculate the distance between [minA, maxA] and [minB, maxB]
		// The distance will be negative if the intervals overlap
float IntervalDistance(float minA, float maxA, float minB, float maxB) {
	if (minA < minB) {
		return minB - maxA;
	}
	else {
		return minA - maxB;
	}
}

// Calculate the projection of a polygon on an axis and returns it as a [min, max] interval
void ProjectPolygon(const Vector2& axis, const Form& polygon, float& min, float& max) {
	// To project a point on an axis use the dot product
	if(polygon.getPoints().empty())
		return;
	float d = axis.Dot(polygon.getPoints()[0]);
	min = d;
	max = d;
	for(const auto& it : polygon.getPoints())
	{
		d = Vector2{ it }.Dot(axis);
		if (d < min) {
			min = d;
		}
		else {
			if (d > max) {
				max = d;
			}
		}
	}
}

	Form::Form(const Form& figure) :
		m_points(figure.m_points), m_edges(figure.m_edges)
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

	Vector2 Form::getCenter()
	{
		float totalX = 0;
		float totalY = 0;
		for(const auto& it : m_points)
		{
			totalX += it.mX;
			totalY += it.mY;
		}
		return { totalX / static_cast<float>(m_points.size()), totalY / static_cast<float>(m_points.size()) };
	}

	void Form::BuildEdges()
	{
		Vector2 p1;
		Vector2 p2;
		m_edges.clear();
		for (int i = 0; i < m_points.size(); i++) {
			p1 = m_points[i];
			if (i + 1 >= m_points.size()) {
				p2 = m_points[0];
			}
			else {
				p2 = m_points[i + 1];
			}
			const auto next_edge = p2 - p1;
			m_edges.emplace_back(FPoint{ next_edge.x, next_edge.y });
		}
	}
	
	//!TODO Left figure intersect right figure?
	bool Form::intersect2D(const Form& left_figure, const FPoint& left_position, const Form& right_figure, const FPoint& right_position)
	{
		bool result = true;
		auto polygonA = left_figure + left_position;
		polygonA.BuildEdges();
		auto polygonB = right_figure + right_position;
		polygonB.BuildEdges();

		const int edgeCountA = polygonA.getEdges().size();
		const int edgeCountB = polygonB.getEdges().size();
		Vector2 edge;

		// Loop through all the edges of both polygons
		for (int edgeIndex = 0; edgeIndex < edgeCountA + edgeCountB; edgeIndex++) 
		{
			if (edgeIndex < edgeCountA) {
				edge = polygonA.getEdges()[edgeIndex];
			}
			else {
				edge = polygonB.getEdges()[edgeIndex - edgeCountA];
			}

			//  Find if the polygons are currently intersecting

			// Find the axis perpendicular to the current edge
			Vector2 axis(edge.x,- edge.y);
			axis = axis.Normalize();

			// Find the projection of the polygon on the current axis
			float minA = 0; float minB = 0; float maxA = 0; float maxB = 0;
			ProjectPolygon(axis, polygonA, minA, maxA);
			ProjectPolygon(axis, polygonB, minB, maxB);

			// Check if the polygon projections are currentlty intersecting
			if (IntervalDistance(minA, maxA, minB, maxB) > 0)
			{
				result = false;
				break;
			}
		}
		return result;
	}

	//!TODO Left figure has plase in right figure?
	bool Form::include2D(const Form& left_figure, const FPoint& left_position, const Form& right_figure, const FPoint& right_position)
	{
		return false;
	}

	bool Form::include2D(const Form& figure, const FPoint& position, const FPoint& point)
	{
		bool result = false;
		size_t j = figure.m_points.size() - 1;
		for (size_t i = 0; i < figure.m_points.size(); i++) {
			const auto point_in_pos = figure.m_points[i] + position;
			if ((point_in_pos.mY < point.mY && point_in_pos.mY >= point.mY) || ((point_in_pos.mY < point.mY && point_in_pos.mY >= point.mY) &&
				point_in_pos.mX + (point.mY - point_in_pos.mY) / (point_in_pos.mY - point_in_pos.mY) * (point_in_pos.mX - point_in_pos.mX) < point.mX))
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
		m_points = std::move(points);
	}

	const std::vector<FPoint>& Form::getPoints() const
	{
		return m_points;
	}

	std::vector<FPoint> Form::getEdges() const
	{
		return m_edges;
	}

	Form Form::operator+(const Vector2& point) const
	{
		Form next(*this);
		for (auto& it : next.m_points)
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
	AABB::AABB(std::initializer_list<FPoint> in_list) :
		Form(in_list)
	{}

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

	bool AABB::intersect2D(const AABB& left_figure, const FPoint& left_position, const AABB& right_figure, const FPoint& right_position)
	{
		return (left_figure + left_position).getRect().Intersects((right_figure + right_position).getRect());
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

	Form3::Form3(const Form3& figure):
		m_points(figure.m_points), m_edges(figure.m_edges)
	{
	}

	Form3::Form3(std::initializer_list<FPoint3> in_list)
	{
		for (const auto& it : in_list)
			m_points.emplace_back(it);
	}

	Form3::Form3(const std::vector<FPoint3>& points):
		m_points(points)
	{
	}

	Form3& Form3::operator=(const Form3& figure)
	{
		m_points = figure.m_points;
		return *this;
	}

	Form3& Form3::operator=(std::initializer_list<FPoint3> in_list)
	{
		m_points = in_list;
		return *this;
	}

	Form3 Form3::operator+(const Vector3& point) const
	{
		Form3 next(*this);
		for (auto& it : next.m_points)
			it += {point.x, point.y, point.z};
		return next;
	}

	Vector3 Form3::getCenter()
	{
		return Vector3();
	}

	void Form3::BuildEdges()
	{
	}

	bool Form3::intersect3D(const Form3& left_figure, const FPoint3& left_position, const Form3& right_figure, const FPoint3& right_position)
	{
		return false;
	}

	bool Form3::include3D(const Form3& left_figure, const FPoint3& left_position, const Form3& right_figure, const FPoint3& right_position)
	{
		return false;
	}

	bool Form3::include3D(const Form3& figure, const FPoint3& position, const FPoint3& point)
	{
		return false;
	}

	void Form3::setPoints(const std::vector<FPoint3>& points)
	{
		m_points = points;
	}

	void Form3::setPoints(std::vector<FPoint3>&& points)
	{
		m_points = std::move(points);
	}

	const std::vector<FPoint3>& Form3::getPoints() const
	{
		return m_points;
	}

	std::vector<FPoint3> Form3::getEdges() const
	{
		return  m_edges;
	}
