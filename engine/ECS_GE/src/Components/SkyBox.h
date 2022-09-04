#pragma once
#include <memory>
#include "VertexArray.h"
namespace RenderEngine
{
	class Texture3D;
	class SkyBox
	{
	public:
		explicit SkyBox(const std::shared_ptr<Texture3D>& texture);
		virtual ~SkyBox() = default;
		SkyBox(const SkyBox&) = delete;
		SkyBox& operator=(const SkyBox&) = delete;
		SkyBox(SkyBox&&) noexcept;
		SkyBox& operator=(SkyBox&&) noexcept;
		virtual std::shared_ptr<Texture3D> setTexture(const std::shared_ptr<Texture3D>& texture);

		const std::shared_ptr<Texture3D>& getTexture3D() const { return m_pTexture; }
		const VertexArray& getVertexArray() const { return m_vertexArray; }
		const VertexBuffer& getVertexCoordsBuffer() const { return m_vertexCoordsBuffer; }
	protected:
		virtual void init();
		std::shared_ptr<Texture3D> m_pTexture;
		VertexArray  m_vertexArray;
		VertexBuffer m_vertexCoordsBuffer;
	};
}