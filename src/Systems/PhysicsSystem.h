#pragma once
#include <memory>
#include <mutex>
#include <vector>

#include "ISystem.h"
#include "Position.h"

class IGameObject;

class PhysicsSystem : public ISystem
{
public:
	friend class DemoServiceLocator;
	void Update(float delta_time);
	bool Registrate(const std::shared_ptr<IGameObject>&);
	bool Unregistrate(const std::shared_ptr<IGameObject>&);
	void Clear();
private:
	void MoveObjects(float delta_time) const;
	void MoveCamera(double x, double y) const;
	void MoveCameraTo(const PositionComponent& pos) const;
	std::vector<std::shared_ptr<IGameObject>> m_objects;
	std::mutex								  m_mutex;
	PhysicsSystem() = default;
	~PhysicsSystem() = default;
	PhysicsSystem(const PhysicsSystem&) = delete;
	PhysicsSystem(PhysicsSystem&&) = delete;
	PhysicsSystem& operator=(const PhysicsSystem&) = delete;
	PhysicsSystem& operator=(PhysicsSystem&&) = delete;
};
