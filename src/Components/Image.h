#pragma once
#include <memory>
#include <string>

#include "Atlas2D.h"
#include "IndexBuffer.h"
#include "Point.h"
#include "VertexArray.h"


namespace RenderEngine
{
	class Texture2D;
	class SpriteAtlas;
	
	class Image2D
	{
	public:
		Image2D() = default;
		Image2D(const std::shared_ptr<Texture2D>& pTexture);

		Image2D(const std::shared_ptr<SpriteAtlas>& pAtlas,
			const SubTexture2D& sub_texture);

		virtual ~Image2D() = default;
		Image2D(const Image2D&);
		Image2D& operator=(const Image2D&);
		Image2D(Image2D&&) noexcept;
		Image2D& operator=(Image2D&&) noexcept;
		FPoint getTextureSize() const;
		std::shared_ptr<Texture2D> getTexture2D() const { return m_pTexture; }
		const VertexArray& getVertexArray() const { return m_vertexArray; }
		const VertexBuffer& getVertexCoordsBuffer() const { return m_vertexCoordsBuffer; }
		const VertexBuffer& getTextureCoordsBuffer() const { return m_textureCoordsBuffer; }
		const IndexBuffer& getIndexCoordsBuffer() const { return m_indexBuffer; }
		void SetSubTexture(const SubTexture2D& sub_texture);
		bool isValid() const;

	protected:
		virtual void init();
		std::shared_ptr<Texture2D> m_pTexture;
		SubTexture2D m_subTexture;
		VertexArray  m_vertexArray;
		VertexBuffer m_vertexCoordsBuffer;
		VertexBuffer m_textureCoordsBuffer;
		IndexBuffer  m_indexBuffer;
	};
}
