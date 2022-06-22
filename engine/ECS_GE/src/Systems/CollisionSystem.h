#pragma once
#include "Collision.h"
#include "ISystem.h"
#include "Position.h"

#define COLLISION CollisionSystem::Instanse()
class CollisionSystem: public ISystem
{
public:
	static CollisionSystem& Instanse();
	//Collision component it is rect without position
	static bool intersect2D(const CollisionComponent& collision_left, const PositionComponent& position_left, const CollisionComponent& collision_right, const PositionComponent& position_right);
	static bool intersect2D(const FRect& rect_left, float left_scale, const PositionComponent& position_left, const FRect& rect_right, float right_scale, const PositionComponent& position_right);
	static bool intersect2D(const CollisionComponent& collision_left, float left_scale, const PositionComponent& position_left, const Vector2& vector2, const PositionComponent& position_right);
	static bool include2D(const CollisionComponent& collision_left, const CollisionComponent& collision_right);
	static bool include2D(const FRect& rect_left, const FRect& rect_right);
	static bool include2D(const FRect& rect_left, const FPoint& point_right);
	static bool include2D(const CollisionComponent& collision_left, const FPoint& point_right);
private:
	CollisionSystem() = default;
	~CollisionSystem() = default;
	CollisionSystem(const CollisionSystem&) = delete;
	CollisionSystem(CollisionSystem&&) = delete;
	CollisionSystem& operator=(const CollisionSystem&) = delete;
	CollisionSystem& operator=(CollisionSystem&&) = delete;
};