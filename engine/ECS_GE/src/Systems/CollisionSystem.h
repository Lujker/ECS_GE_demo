#pragma once
#include "Collision.h"
#include "ISystem.h"
#include "Position.h"

class CollisionSystem: public ISystem
{
public:
	friend class ServiceLocator;
	//Collision component it is rect without position
	bool intersect2D(const CollisionComponent& collision_left, const PositionComponent& position_left, const CollisionComponent& collision_right, const PositionComponent& position_right);
	bool intersect2D(const FRect& rect_left, float left_scale, const PositionComponent& position_left, const FRect& rect_right, float right_scale, const PositionComponent& position_right);
	bool intersect2D(const CollisionComponent& collision_left, float left_scale, const PositionComponent& position_left, const Vector2& vector2, const PositionComponent& position_right);
	bool include2D(const CollisionComponent& collision_left, const CollisionComponent& collision_right);
	bool include2D(const FRect& rect_left, const FRect& rect_right);
	bool include2D(const FRect& rect_left, const FPoint& point_right);
	bool include2D(const CollisionComponent& collision_left, float left_scale, const PositionComponent& position_left, const FPoint& point_right);

	CollisionSystem(const CollisionSystem&) = delete;
	CollisionSystem(CollisionSystem&&) = delete;
	CollisionSystem& operator=(const CollisionSystem&) = delete;
	CollisionSystem& operator=(CollisionSystem&&) = delete;
private:
	CollisionSystem() = default;
	~CollisionSystem() override = default;
};