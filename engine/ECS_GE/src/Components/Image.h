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
		const VertexBuffer& getNormalCoordsBuffer() const { return m_normalCoordsBuffer; }
		const VertexBuffer& getTextureCoordsBuffer() const { return m_textureCoordsBuffer; }
		const IndexBuffer& getIndexCoordsBuffer() const { return m_indexBuffer; }
		virtual void SetSubTexture(const SubTexture2D& sub_texture);
		virtual bool isValid() const;
		virtual bool isMirrored() const;
		virtual void mirror(bool vertical, bool horizontal);
		virtual void updateVertex(const void* data, const unsigned int data_size);
		virtual bool isVerticalMirror() const { return m_isVerticalMirror; }
		virtual bool isHorizontalMirror() const { return m_isHorizontalMirror; }

	protected:
		virtual void init();
		std::shared_ptr<Texture2D> m_pTexture;
		SubTexture2D m_subTexture;
		VertexArray  m_vertexArray;
		VertexBuffer m_vertexCoordsBuffer;
		VertexBuffer m_normalCoordsBuffer;
		VertexBuffer m_textureCoordsBuffer;
		IndexBuffer  m_indexBuffer;
		bool		 is_mirrored = false;
		bool		 m_isHorizontalMirror = false;
		bool		 m_isVerticalMirror = false;
	};
}
