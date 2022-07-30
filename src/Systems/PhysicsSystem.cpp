#include "PhysicsSystem.h"
#include <future>

#include "CameraManager.h"
#include "CollisionSystem.h"
#include "GameObject.h"
#include "MoveSystem.h"
#include "ThreadPool.h"

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
	std::vector<std::future<void>> futures;
	for (auto& it : m_objects)
	{
		//Calculate next move and next pos for obj
		auto chek_obj = [&](const std::shared_ptr<IGameObject>& pointer) {
			auto next_move = pointer->GetMove();
			if (pointer->gravityEnable())
				next_move = MOVE.Gravity(next_move, delta_time);
			next_move = MOVE.CalculateVelocity(next_move, delta_time);
			auto next_pos = MOVE.Move(pointer->GetPosition(), next_move, delta_time);

			std::shared_ptr<IGameObject> collisionX = nullptr, collisionY = nullptr;
			// And check collision with other objects
			if (bool cantMoveX = false, cantMoveY = false; pointer->GetCollision().hasCollision())
			{
				for (auto& sec_it : m_objects)
				{
					if (cantMoveX && cantMoveY)
						break;
					if (sec_it == pointer)
						continue;
					//! Check intersect with other obj by x coord
					if (!cantMoveX)
					{
						cantMoveX = COLLISION.intersect2D(pointer->GetCollision(), PositionComponent{ {next_pos.getPosition().mX, pointer->GetPosition().getPosition().mY}, pointer->GetPosition().getLayer() },
							sec_it->GetCollision(), sec_it->GetPosition());
						if (cantMoveX)
							collisionX = sec_it;
					}
					//! And check by y
					if (!cantMoveY)
					{
						cantMoveY = COLLISION.intersect2D(pointer->GetCollision(), PositionComponent{ {pointer->GetPosition().getPosition().mX,  next_pos.getPosition().mY}, pointer->GetPosition().getLayer() },
							sec_it->GetCollision(), sec_it->GetPosition());
						if (cantMoveY)
							collisionY = sec_it;
					}
				}
				auto pos_x = next_pos.getPosition().mX, pos_y = next_pos.getPosition().mY;
				auto vel_x = next_move.getVelocity().x, vel_y = next_move.getVelocity().y;
				auto acs_x = next_move.getAcceleration().x, acs_y = next_move.getAcceleration().y;
				if (cantMoveX)
				{
					pos_x = pointer->GetPosition().getPosition().mX;
					vel_x = pointer->GetMove().getVelocity().x;
					acs_x = 0.f;
					if (collisionX)
					{
						if (collisionX->GetPosition().getPosition().mX > pointer->GetPosition().getPosition().mX + pointer->GetCollision().getWidth())
							pos_x = collisionX->GetPosition().getPosition().mX - pointer->GetCollision().getWidth();
						else if (collisionX->GetPosition().getPosition().mX + collisionX->GetCollision().getWidth() < pointer->GetPosition().getPosition().mX)
							pos_x = collisionX->GetPosition().getPosition().mX + collisionX->GetCollision().getWidth();
					}
				}
				if (cantMoveY)
				{
					pos_y = pointer->GetPosition().getPosition().mY;
					vel_y = 0.f;
					acs_y = 0.f;
					if (collisionY)
					{
						if (collisionY->GetPosition().getPosition().mY > pointer->GetPosition().getPosition().mY + pointer->GetCollision().getHeight())
							pos_y = collisionY->GetPosition().getPosition().mY - pointer->GetCollision().getHeight();
						else if (collisionY->GetPosition().getPosition().mY + collisionY->GetCollision().getHeight() < pointer->GetPosition().getPosition().mY)
							pos_y = collisionY->GetPosition().getPosition().mY + collisionY->GetCollision().getHeight();
					}
				}
				next_pos = { {pos_x, pos_y}, pointer->GetPosition().getLayer(), pointer->GetPosition().getRotation() };
				next_move = { pointer->GetMove().getDirection(), {vel_x, vel_y}, {acs_x, acs_y} };
			}
			//! if move comp change
			next_move.UpdateDirection();
			if (pointer->GetMove() != next_move)
			{
				pointer->SetMove(next_move);
				pointer->MoveChange();
			}
			//! if pos comp change
			if (pointer->GetPosition() != next_pos)
			{
				pointer->SetPosition(next_pos);
				pointer->PositionChange();
				//! If camera centrailyze to this obj
				if (pointer->cameraObject())
				{
					MoveCameraTo(next_pos);
				}
			}
		};
		//! multithread
		futures.emplace_back(ASYNC.submit([chek_obj, it] { return chek_obj(it); }));
		//!Thread_pool t_p; 
		//! t_p.submit(...) or ->
		//
		//!auto func = std::async(std::launch::async, chek_obj, it);
		//
		//! singlthread
		//!chek_obj(it);
	}
	for (const auto& future : futures)
	{
		future.wait();
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
