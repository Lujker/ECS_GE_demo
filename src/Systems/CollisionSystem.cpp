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
		return intersect2D(collision_left.getRect(), position_left, collision_right.getRect(), position_right);
	auto l_form = collision_left.getForm()->getPoints();
	for (auto& point : l_form)
	{
		point.mX *= collision_left.getScale(); point.mY *= collision_left.getScale();
	}
	auto r_form = collision_right.getForm()->getPoints();
	for (auto& point : r_form)
	{
		point.mX *= collision_right.getScale(); point.mY *= collision_right.getScale();
	}
	return Form::intersect2D(l_form, position_left.getPosition(), r_form, position_right.getPosition());
}

bool CollisionSystem::intersect2D(const FRect& rect_left, float left_scale, const PositionComponent& position_left, const FRect& rect_right, float right_scale, const PositionComponent& position_right)
{
	auto l_r = rect_left; auto r_r = rect_right;
	l_r += position_left.getPosition(); l_r.mWidth *= left_scale;
	r_r += position_right.getPosition(); r_r.mWidth *= right_scale;
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

bool CollisionSystem::include2D(const CollisionComponent& collision_left, const FPoint& point_right)
{
	return false;
}
