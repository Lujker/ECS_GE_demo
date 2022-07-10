#include "PhysicsSystem.h"

#include "CameraManager.h"
#include "CollisionSystem.h"
#include "GameObject.h"
#include "MoveSystem.h"

PhysicsSystem& PhysicsSystem::Instance()
{
	static PhysicsSystem physics_system;
	return physics_system;
}

void PhysicsSystem::Update(float delta_time)
{
	std::lock_guard l_g(m_mutex);
	MoveObjects(delta_time);
}

void PhysicsSystem::Clear()
{
	std::lock_guard l_g(m_mutex);
	m_objects.clear();
}

void PhysicsSystem::MoveObjects(float delta_time) const
{
	//!Need oprimizations!
	for (auto& it : m_objects)
	{
		//Calculate next move and next pos for obj
		auto next_move = it->GetMove();
		if (it->gravityEnable())
			next_move = MOVE.Gravity(next_move, delta_time);
		next_move = MOVE.CalculateVelocity(next_move, delta_time);
		auto next_pos = MOVE.Move(it->GetPosition(), next_move, delta_time);
		
		std::shared_ptr<IGameObject> collisionX = nullptr, collisionY = nullptr;
		// And check collision with other objects
		if (bool cantMoveX = false, cantMoveY = false; it->GetCollision().hasCollision())
		{
			for (auto& sec_it : m_objects)
			{
				if (cantMoveX && cantMoveY)
					break;
				if (sec_it == it)
					continue;
				//! Check intersect with other obj by x coord
				if (!cantMoveX)
				{
					cantMoveX = COLLISION.intersect2D(it->GetCollision(), PositionComponent{ {next_pos.getPosition().mX, it->GetPosition().getPosition().mY}, it->GetPosition().getLayer() },
						sec_it->GetCollision(), sec_it->GetPosition());
					collisionX = sec_it;
				}
				//! And check by y
				if (!cantMoveY)
				{
					cantMoveY = COLLISION.intersect2D(it->GetCollision(), PositionComponent{ {it->GetPosition().getPosition().mX,  next_pos.getPosition().mY}, it->GetPosition().getLayer() },
						sec_it->GetCollision(), sec_it->GetPosition());
					collisionY = sec_it;
				}
			}
			auto pos_x = next_pos.getPosition().mX, pos_y = next_pos.getPosition().mY;
			auto vel_x = next_move.getVelocity().x, vel_y = next_move.getVelocity().y;
			auto acs_x = next_move.getAcceleration().x, acs_y = next_move.getAcceleration().y;
			if (cantMoveX)
			{
				pos_x = it->GetPosition().getPosition().mX;
				vel_x = it->GetMove().getVelocity().x;
				acs_x = 0.f;
				if (collisionX)
				{
					if (collisionX->GetPosition().getPosition().mX >= it->GetPosition().getPosition().mX + it->GetCollision().getWidth())
						pos_x = collisionX->GetPosition().getPosition().mX;
					else if (collisionX->GetPosition().getPosition().mX + collisionX->GetCollision().getWidth() <= it->GetPosition().getPosition().mX)
						pos_x = collisionX->GetPosition().getPosition().mX + collisionX->GetCollision().getWidth();
				}
			}
			if (cantMoveY)
			{
				pos_y = it->GetPosition().getPosition().mY;
				vel_y = 0.f;
				acs_y = 0.f;
				if (collisionY)
				{
					if (collisionY->GetPosition().getPosition().mY >= it->GetPosition().getPosition().mY + it->GetCollision().getHeight())
						pos_y = collisionY->GetPosition().getPosition().mY;
					else if (collisionY->GetPosition().getPosition().mY + collisionY->GetCollision().getHeight() <= it->GetPosition().getPosition().mY)
						pos_y = collisionY->GetPosition().getPosition().mY + collisionY->GetCollision().getHeight();
				}
			}
			next_pos = { {pos_x, pos_y}, it->GetPosition().getLayer(), it->GetPosition().getRotation() };
			next_move = { it->GetMove().getDirection(), {vel_x, vel_y}, {acs_x, acs_y} };
		}
		//! if move comp change
		next_move.UpdateDirection();
		if (it->GetMove() != next_move)
		{
			it->SetMove(next_move);
			it->MoveChange();
		}
		//! if pos comp change
		if (it->GetPosition() != next_pos) 
		{
			it->SetPosition(next_pos);
			it->PositionChange();
			//! If camera centrailyze to this obj
			if (it->cameraObject())
			{
				MoveCameraTo(next_pos);
			}
		}
	}
}

void PhysicsSystem::MoveCamera(double x, double y) const
{
	CAMERA.Move(x, y);
}

void PhysicsSystem::MoveCameraTo(const PositionComponent& pos) const
{
	CAMERA.SetCenterPoint({ pos.getPosition().mX, pos.getPosition().mY });
}

bool PhysicsSystem::Registrate(const std::shared_ptr<IGameObject>& object)
{
	std::lock_guard l_g(m_mutex);
	const auto it = std::find(m_objects.begin(), m_objects.end(), object);
	if( it == m_objects.end())
	{
		m_objects.emplace_back(object);
		return true;
	}
	return false;
}

bool PhysicsSystem::Unregistrate(const std::shared_ptr<IGameObject>& object)
{
	std::lock_guard l_g(m_mutex);
	auto it = std::find(m_objects.begin(), m_objects.end(), object);
	if (it != m_objects.end())
	{
		m_objects.erase(std::remove_if(m_objects.begin(), m_objects.end(), [&](const auto& first)
		{
				return (*first) == (*(*it).get());
		}), m_objects.end());
		return true;
	}
	return false;
}
