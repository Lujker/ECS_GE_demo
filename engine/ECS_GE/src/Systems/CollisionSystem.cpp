#include "CollisionSystem.h"

CollisionSystem& CollisionSystem::Instanse()
{
	static CollisionSystem col_system;
	return col_system;
}

bool CollisionSystem::intersect2D(const CollisionComponent& collision_left, const PositionComponent& position_left, const CollisionComponent& collision_right, const PositionComponent& position_right)
{
	if (!collision_left.isValid() || !collision_right.isValid())
		return false;
	if (collision_left.isRect() && collision_right.isRect())
		return intersect2D(collision_left.getRect(), collision_left.getScale(),position_left, collision_right.getRect(), collision_right.getScale(), position_right);
	auto l_form = collision_left.getForm()->getPoints();
	auto r_form = collision_right.getForm()->getPoints();
	if (collision_left.getScale() != 1.f && collision_left.getScale() != 0.f)
	{
		for (auto& point : l_form)
		{
			point.mX *= collision_left.getScale(); point.mY *= collision_left.getScale();
		}
	}
	if (collision_right.getScale() != 1.f && collision_right.getScale() != 0.f)
	{
		for (auto& point : r_form)
		{
			point.mX *= collision_right.getScale(); point.mY *= collision_right.getScale();
		}
	}
	return Form::intersect2D(l_form, position_left.getPosition(), r_form, position_right.getPosition());
}

bool CollisionSystem::intersect2D(const FRect& rect_left, float left_scale, const PositionComponent& position_left, const FRect& rect_right, float right_scale, const PositionComponent& position_right)
{
	auto l_r = rect_left; auto r_r = rect_right;
	l_r += position_left.getPosition();
	r_r += position_right.getPosition();
	if (left_scale != 1.f && left_scale != 0.f)
	{
		l_r.mWidth *= left_scale; l_r.mHeight *= left_scale;
	}
	if (right_scale != 1.f && right_scale != 0.f)
	{
		r_r.mWidth *= right_scale; r_r.mHeight *= right_scale;
	}
	return l_r.Intersects(r_r);
}

bool CollisionSystem::intersect2D(const CollisionComponent& collision_left, float left_scale, const PositionComponent& position_left, const Vector2& vector2, const PositionComponent& position_right)
{
	return false;
}

bool CollisionSystem::include2D(const CollisionComponent& collision_left, const CollisionComponent& collision_right)
{
	return false;
}

bool CollisionSystem::include2D(const FRect& rect_left, const FRect& rect_right)
{
	return false;
}

bool CollisionSystem::include2D(const FRect& rect_left, const FPoint& point_right)
{
	return rect_left.Contains(point_right);
}

bool CollisionSystem::include2D(const CollisionComponent& collision_left, float left_scale,const PositionComponent& position_left, const FPoint& point_right)
{
	if (collision_left.isValid() && collision_left.isRect())
	{
		auto r = collision_left.getRect();
		r += position_left.getPosition();
		if (left_scale != 1.f && left_scale != 0.f)
		{
			r.mWidth *= left_scale; r.mHeight *= left_scale;
		}
		return include2D(r, point_right);
	}
	return false;
}
