#pragma once
#include "Vector.h"
#include "Cube.h"
#include <memory>
#include "Position.h"
#include "Collision.h"
#include <unordered_map>

namespace RenderEngine
{
	struct Light
	{
		RenderEngine::LightCube lightCube;
		Vector3     ambient = { 1.f, 1.f, 1.f };
		Vector3     diffuse = { 1.f, 1.f, 1.f };
		Vector3     specular = { 1.f, 1.f, 1.f };
		float       shininess = 16.f;
		PositionComponent3 position;
		CollisionComponent3 size;
	};

	class LightManager
	{
	public:
		friend class ServiceLocator;
		using light_map = std::unordered_map<std::string, std::shared_ptr<Light>>;

		void pushLight(const std::string& name, const std::shared_ptr<Light>& light);
		std::shared_ptr<Light> getLight(const std::string& name);
		const light_map& getLights();
		void clearLights();

		void Draw();

		LightManager(const LightManager&) = delete;
		LightManager(LightManager&&) = delete;
		LightManager& operator=(const LightManager&) = delete;
		LightManager& operator=(LightManager&&) = delete;
	private:
		LightManager() = default;
		~LightManager() = default;
		void init();
		void destroy();

		light_map light_objects;
	};
}