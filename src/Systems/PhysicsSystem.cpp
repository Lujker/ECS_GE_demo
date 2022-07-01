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
		auto next_pos = MOVE.Move(it->GetPosition(), it->GetMove(), delta_time);
		if (it->GetCollision().hasCollision())
		{
			bool canMoveX = false, canMoveY = false;
			for (auto& sec_it : m_objects)
			{
				if (canMoveX && canMoveY)
					break;
				if (sec_it == it)
					continue;
				if (!canMoveX)
					canMoveX = COLLISION.intersect2D(it->GetCollision(), PositionComponent{ {next_pos.getPosition().mX, it->GetPosition().getPosition().mY}, it->GetPosition().getLayer() },
						sec_it->GetCollision(), sec_it->GetPosition());
				if (!canMoveY)
					canMoveY = COLLISION.intersect2D(it->GetCollision(), PositionComponent{ {it->GetPosition().getPosition().mX,  next_pos.getPosition().mY}, it->GetPosition().getLayer() },
						sec_it->GetCollision(), sec_it->GetPosition());
			}
			if (canMoveX)
			{
				next_pos = { {it->GetPosition().getPosition().mX, next_pos.getPosition().mY}, it->GetPosition().getLayer(), it->GetPosition().getRotation() };
			}
			if (canMoveY)
			{
				next_pos = { {next_pos.getPosition().mX,it->GetPosition().getPosition().mY }, it->GetPosition().getLayer(), it->GetPosition().getRotation() };
			}
		}
		//! need if(it->IsCameraMan()) MoveCamera(nextPost - it.pos);
		it->SetPosition(next_pos);
		it->MoveCallback();
	}
}

void PhysicsSystem::MoveCamera(int x, int y) const
{
	//CAMERA.Move(next_pos.getPosition().mX - m_position.getPosition().mX, next_pos.getPosition().mY - m_position.getPosition().mY);
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
