#include "CollisionSystem.h"

CollisionSystem& CollisionSystem::Instanse()
{
	static CollisionSystem col_system;
	return col_system;
}

bool CollisionSystem::intersect2D(const CollisionComponent& collision_left, const CollisionComponent& collision_right)
{
	return false;
}

bool CollisionSystem::intersect2D(const FRect& rect_left, const FRect& rect_right)
{
	return rect_left.Intersects(rect_right);
}

bool CollisionSystem::intersect2D(const CollisionComponent& collision_left, const Vector2& vector2)
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
