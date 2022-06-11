#pragma once
#include "Collision.h"

#define COLLISION CollisionSystem::Instanse()
class CollisionSystem
{
public:
	static CollisionSystem& Instanse();
	//Collision component it is rect without position
	static bool intersect2D(const CollisionComponent& collision_left, const CollisionComponent& collision_right);
	static bool intersect2D(const FRect& rect_left, const FRect& rect_right);
	static bool intersect2D(const CollisionComponent& collision_left, const Vector2& vector2);
	static bool include2D(const CollisionComponent& collision_left, const CollisionComponent& collision_right);
	static bool include2D(const FRect& rect_left, const FRect& rect_right);
	static bool include2D(const FPoint& point_left, const FPoint& point_right);
private:
	CollisionSystem() = default;
	~CollisionSystem() = default;
	CollisionSystem(const CollisionSystem&) = delete;
	CollisionSystem(CollisionSystem&&) = delete;
	CollisionSystem& operator=(const CollisionSystem&) = delete;
	CollisionSystem& operator=(CollisionSystem&&) = delete;
};