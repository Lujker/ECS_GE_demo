#include "LightManager.h"
#include "Engine.h"
#include "RenderSystem.h"

namespace RenderEngine {

	void LightManager::clearLights()
	{
		light_objects.clear();
		m_point_lights_count = 0;
		m_directions_lights_count = 0;
	}

	void LightManager::pushLight(const std::string& name, const std::shared_ptr<Light>& light)
	{
		switch (light->type)
		{
		case RenderEngine::LightType::eDirection:
			++m_directions_lights_count;
			break;
		case RenderEngine::LightType::ePoint:
			++m_point_lights_count;
			break;
		default:
			break;
		}
		light_objects.insert({ name, light });
	}

	std::shared_ptr<Light> LightManager::getLight(const std::string& name)
	{
		auto it = light_objects.find(name);
		if (it != light_objects.end())
		{
			return it->second;
		}
		return nullptr;
	}

	void LightManager::init()
	{

	}

	void LightManager::destroy()
	{
		clearLights();

	}

	const LightManager::light_map& LightManager::getLights()
	{
		return light_objects;
	}

	void LightManager::Draw()
	{
		for (const auto& it : light_objects)
		{
			RENDER->Render(it.second);
		}
	}
	Light::~Light()
	{}

	PointLight::PointLight(
		PositionComponent3 position, 
		CollisionComponent3 size, 
		Vector3 ambient, 
		Vector3 diffuse, 
		Vector3 specular, 
		float shininess,
		float constant,
		float linear,
		float quadratic) :
		position(position), constant(constant), linear(linear), quadratic(quadratic)
	{
		Light::type = LightType::ePoint;
		Light::ambient = ambient;
		Light::diffuse = diffuse;
		Light::specular = specular;
		Light::size = size;
	}

	DirectionLight::DirectionLight(
		Vector3 direction, 
		CollisionComponent3 size, 
		Vector3 ambient, 
		Vector3 diffuse,
		Vector3 specular, 
		float shininess):
		direction(direction)
	{
		Light::type = LightType::eDirection;
		Light::ambient = ambient;
		Light::diffuse = diffuse;
		Light::specular = specular;
		Light::size = size;
	}

}