#pragma once
#include <vector>
#include <memory>
#include "VertexArray.h"
#include "Collor.h"
#include "Material.h"

namespace RenderEngine {
	class Cube
	{
	public:
		Cube() = default;
		Cube(const std::shared_ptr<Texture2D>&pTexture, std::shared_ptr<Texture2D> specularMap = nullptr);

		~Cube() = default;
		Cube(const Cube&);
		Cube& operator=(const Cube&);
		Cube(Cube&&) noexcept;
		Cube& operator=(Cube&&) noexcept;

		void SetSpecularMap(const std::shared_ptr<Texture2D>&);
		void SetDiffuseTexture(const std::shared_ptr<Texture2D>&);
		void SetShininess(float shininess);
		void updateBuffer(const void* data, const unsigned int data_size);
		const TextureMaterial& getMaterial() { return m_material; }
		const VertexArray& getVertexArray() const { return m_vertexArray; }
		const VertexBuffer& getVertexCoordsBuffer() const { return m_vertexCoordsBuffer; }

	private:
		void init();
		TextureMaterial					m_material;
		VertexArray						m_vertexArray;
		VertexBuffer					m_vertexCoordsBuffer;
	};

	class LightCube
	{
	public:
		LightCube(const ColorComponent& color = ColorComponent{});
		~LightCube() = default;
		LightCube(const LightCube&) = delete;
		LightCube& operator=(const LightCube&) = delete;
		LightCube(LightCube&&) noexcept;
		LightCube& operator=(LightCube&&) noexcept;

		const VertexArray& getVertexArray() const { return m_vertexArray; }
		const VertexBuffer& getVertexCoordsBuffer() const { return m_vertexCoordsBuffer; }
		ColorComponent getColor() const { return m_color; }
		ColorComponent setColor(const ColorComponent& color);
	private:
		void init();
		ColorComponent	m_color;
		VertexArray		m_vertexArray;
		VertexBuffer	m_vertexCoordsBuffer;
	};
}