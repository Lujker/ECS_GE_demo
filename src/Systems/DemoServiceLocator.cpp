#include "DemoServiceLocator.h"
#include "PhysicsSystem.h"


DemoServiceLocator::~DemoServiceLocator()
{
    destroyDemoServiceLocator();
}

void DemoServiceLocator::init(const std::string& init_path)
{
    destroyDemoServiceLocator();
    ServiceLocator::init(init_path);
    p_physics_system = new PhysicsSystem;
}

void DemoServiceLocator::destroyDemoServiceLocator()
{
    delete p_physics_system;
}