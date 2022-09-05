#pragma once
#include <vector>
#include <memory>
#include "Collor.h"
#include "Material.h"
#include "VertexArray.h"
#include "IndexBuffer.h"

namespace RenderEngine {


	class Sphere
	{
	public:
		Sphere();
		Sphere(const std::shared_ptr<Texture2D>& pTexture, std::shared_ptr<Texture2D> specularMap = nullptr);

		~Sphere() = default;
		Sphere(const Sphere&);
		Sphere& operator=(const Sphere&);
		Sphere(Sphere&&) noexcept;
		Sphere& operator=(Sphere&&) noexcept;

		void SetSpecularMap(const std::shared_ptr<Texture2D>&);
		void SetDiffuseTexture(const std::shared_ptr<Texture2D>&);
		void SetShininess(float shininess);
		void updateBuffer(const void* data, const unsigned int data_size);
		const TextureMaterial& getMaterial() { return m_material; }
		const VertexArray& getVertexArray() const { return m_vertexArray; }
		const VertexBuffer& getVertexCoordsBuffer() const { return m_vertexCoordsBuffer; }
		const IndexBuffer& getIndexCoordsBuffer() const { return m_indexCoordsBuffer; }

	private:
		void init();
		TextureMaterial					m_material;
		VertexArray						m_vertexArray;
		VertexBuffer					m_vertexCoordsBuffer;
		VertexBuffer					m_normalsCoordsBuffer;
		VertexBuffer					m_texturesCoordsBuffer;
		IndexBuffer						m_indexCoordsBuffer;
	};
}