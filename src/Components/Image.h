#include <memory>

#include "Atlas2D.h"
#include "IndexBuffer.h"
#include "ShaderProgram.h"
#include "texture2D.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "Rect.h"

namespace RenderEngine
{
	class Image2D
	{
	public:
		Image2D(const std::shared_ptr<Texture2D>& pTexture);

		Image2D(const std::shared_ptr<TextureAtlas>& pAtlas,
			const std::string& initialSubTextureName = "");

		virtual ~Image2D() = default;
		Image2D(const Image2D&);
		Image2D& operator=(const Image2D&);
		Image2D(Image2D&&) noexcept;
		Image2D& operator=(Image2D&&) noexcept;
		std::shared_ptr<Texture2D> getTexture2D() const { return m_pTexture; }
		const VertexArray& getVertexArray() const { return m_vertexArray; }
		const VertexBuffer& getVertexCoordsBuffer() const { return m_vertexCoordsBuffer; }
		const VertexBuffer& getTextureCoordsBuffer() const { return m_textureCoordsBuffer; }
		const IndexBuffer& getIndexCoordsBuffer() const { return m_indexBuffer; }

	protected:
		virtual void init();
		std::shared_ptr<Texture2D> m_pTexture;
		std::shared_ptr<TextureAtlas> m_pAtlas;
		SubTexture2D m_subTexture;
		VertexArray  m_vertexArray;
		VertexBuffer m_vertexCoordsBuffer;
		VertexBuffer m_textureCoordsBuffer;
		IndexBuffer  m_indexBuffer;
	};
}
