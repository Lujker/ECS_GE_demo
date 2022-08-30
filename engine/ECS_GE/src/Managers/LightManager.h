#pragma once
#include "Vector.h"
#include "Cube.h"
#include <memory>
#include "Position.h"
#include "Collision.h"
#include <unordered_map>

namespace RenderEngine
{
	enum class LightType : unsigned
	{
		eNone = 0,
		ePoint = 1,
		eDirection = 2
	};

	struct Light
	{
		virtual ~Light() = 0;
		LightType	type;
		Vector3     ambient = { 1.f, 1.f, 1.f };
		Vector3     diffuse = { 1.f, 1.f, 1.f };
		Vector3     specular = { 1.f, 1.f, 1.f };
		CollisionComponent3 size;
		RenderEngine::LightCube lightCube;
	};

	struct PointLight : public Light
	{
		PointLight(PositionComponent3 position = { 0.f, 0.f, 0.f },
					CollisionComponent3 size = { 10.f, 10.f, 10.f },
					Vector3	ambient = { 1.f, 1.f, 1.f },
					Vector3 diffuse = { 1.f, 1.f, 1.f },
					Vector3 specular = { 1.f, 1.f, 1.f },
					float	shininess = 16.f,
					float constant = 1.0f,
					float linear = 0.007f,
					float quadratic = 0.0002f);
		~PointLight() override = default;
		PositionComponent3 position;
		float constant;
		float linear;
		float quadratic;
	};

	struct DirectionLight : public Light
	{
		DirectionLight(	Vector3 direction = { -0.2f, -1.0f, 0.2f },
						CollisionComponent3 size = { 10.f, 10.f, 10.f },
						Vector3	ambient = { 1.f, 1.f, 1.f },
						Vector3 diffuse = { 1.f, 1.f, 1.f },
						Vector3 specular = { 1.f, 1.f, 1.f },
						float	shininess = 16.f);
		~DirectionLight() override = default;
		Vector3 direction { 1.f, 1.f, 1.f };
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
		unsigned getPointsLightsCount() { return m_point_lights_count; }
		unsigned getDirectionsLightsCount() { return m_directions_lights_count; }

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
		unsigned m_point_lights_count = 0;
		unsigned m_directions_lights_count = 0;
		light_map light_objects;
	};
}