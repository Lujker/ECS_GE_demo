#include "LightManager.h"
#include "Engine.h"
#include "RenderSystem.h"

namespace RenderEngine {

	void LightManager::clearLights()
	{
		light_objects.clear();
	}

	void LightManager::pushLight(const std::string& name, const std::shared_ptr<Light>& light)
	{
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
}