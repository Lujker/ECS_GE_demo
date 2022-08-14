#pragma once
#include <string>
#include "ServiceLocator.h"
#include "Engine.h"

class PhysicsSystem;

#define PHYSICS static_cast<DemoServiceLocator*>(Engine::getServiceLocator())->getPhysicsSystem()
class DemoServiceLocator : public ServiceLocator
{
public:
	DemoServiceLocator() = default;
	~DemoServiceLocator() override;
	void init(const std::string& init_path) override;
	[[nodiscard]] PhysicsSystem* getPhysicsSystem() const { return p_physics_system; }

	void	setPhysicsSystem(PhysicsSystem* physics_system) { p_physics_system = physics_system; }

	DemoServiceLocator(const DemoServiceLocator&) = delete;
	DemoServiceLocator(DemoServiceLocator&&) = delete;
	DemoServiceLocator& operator=(const DemoServiceLocator&) = delete;
	DemoServiceLocator& operator=(DemoServiceLocator&&) = delete;
private:
	void destroyDemoServiceLocator();
	PhysicsSystem* p_physics_system = nullptr;
};