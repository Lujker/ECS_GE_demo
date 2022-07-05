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
		if (it->IsGravityObject())
			next_move = MOVE.Gravity(next_move, delta_time);
		next_move = MOVE.CalculateVelocity(next_move, delta_time);
		auto next_pos = MOVE.Move(it->GetPosition(), it->GetMove(), delta_time);
		// And check collision with other objects
		if (it->GetCollision().hasCollision())
		{
			bool cantMoveX = false, cantMoveY = false;
			for (auto& sec_it : m_objects)
			{
				if (cantMoveX && cantMoveY)
					break;
				if (sec_it == it)
					continue;
				//! Check intersect with other obj by x coord
				if (!cantMoveX)
					cantMoveX = COLLISION.intersect2D(it->GetCollision(), PositionComponent{ {next_pos.getPosition().mX, it->GetPosition().getPosition().mY}, it->GetPosition().getLayer() },
						sec_it->GetCollision(), sec_it->GetPosition());
				//! And check by y
				if (!cantMoveY)
					cantMoveY = COLLISION.intersect2D(it->GetCollision(), PositionComponent{ {it->GetPosition().getPosition().mX,  next_pos.getPosition().mY}, it->GetPosition().getLayer() },
						sec_it->GetCollision(), sec_it->GetPosition());
			}
			if (cantMoveX)
			{
				next_pos = { {it->GetPosition().getPosition().mX, next_pos.getPosition().mY}, it->GetPosition().getLayer(), it->GetPosition().getRotation() };
				next_move = { it->GetMove().getDirection(), {it->GetMove().getVelocity().x, next_move.getVelocity().y}, {0.f, next_move.getAcceleration().y} };
			}
			if (cantMoveY)
			{
				next_pos = { {next_pos.getPosition().mX,it->GetPosition().getPosition().mY }, it->GetPosition().getLayer(), it->GetPosition().getRotation() };
				next_move = { it->GetMove().getDirection(), {next_move.getVelocity().x, 0.f}, {next_move.getAcceleration().x, 0.f} };
			}
		}
		//! if move comp change
		if (it->GetMove() != next_move)
		{
			next_move.UpdateDirection();
			it->SetMove(next_move);
			it->MoveChange();
		}
		//! if pos comp change
		if (it->GetPosition() != next_pos) 
		{
			it->SetPosition(next_pos);
			it->PositionChange();
			//! If camera centrailyze to this obj
			if (it->IsCameraObject())
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
