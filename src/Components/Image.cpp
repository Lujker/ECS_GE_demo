#include "Image.h"

#include "Rect.h"
#include "ResourceManager.h"
#include "texture2D.h"

namespace RenderEngine
{
	Image2D::Image2D(const std::shared_ptr<Texture2D>& pTexture):
		m_pTexture(pTexture)
	{
		m_subTexture = SubTexture2D();
		init();
	}

	Image2D::Image2D(const std::shared_ptr<TextureAtlas>& pAtlas, const std::string& initialSubTextureName):
		m_pTexture(pAtlas)
	{
		m_subTexture = pAtlas->getSubTexture(initialSubTextureName);
		init();
	}

	Image2D::Image2D(const Image2D& image):
		m_pTexture(image.m_pTexture), m_subTexture(image.m_subTexture)
		
	{
		init();
	}

	Image2D& Image2D::operator=(const Image2D& image)
	{
		m_pTexture = image.m_pTexture;
		m_subTexture = image.m_subTexture;
		init();
		return *this;
	}

	Image2D::Image2D(Image2D&& image) noexcept :
		m_pTexture(std::move(image.m_pTexture)), m_subTexture(image.m_subTexture),
		m_vertexArray(std::move(image.m_vertexArray)), m_vertexCoordsBuffer(std::move(image.m_vertexCoordsBuffer)),
		m_textureCoordsBuffer(std::move(image.m_textureCoordsBuffer)), m_indexBuffer(std::move(image.m_indexBuffer))
	{}

	Image2D& Image2D::operator=(Image2D&& image) noexcept
	{
		m_pTexture = std::move(image.m_pTexture);
		m_subTexture = image.m_subTexture;
		m_vertexArray = std::move(image.m_vertexArray);
		m_vertexCoordsBuffer = std::move(image.m_vertexCoordsBuffer);
		m_textureCoordsBuffer = std::move(image.m_textureCoordsBuffer);
		m_indexBuffer = std::move(image.m_indexBuffer);
		return *this;
	}

	FPoint Image2D::getTextureSize() const
	{
		if(m_pTexture)
		{
			return FPoint{ static_cast<double>(m_pTexture->getWidth()), static_cast<double>(m_pTexture->getHeight()) };
		}
		return FPoint();
	}

	//void Image2D::render(FRect rect, float rotation, float layer) const
	//{
	//	m_pShaderProgram->use();
	//	m_pShaderProgram->setMatrix4("modelMatrix", RENDER.getTransformMatrix(rect.mX, rect.mY, rect.mWidth, rect.mHeight, rotation));
	//	m_pShaderProgram->setFloat("layer", layer);
	//	m_pShaderProgram->setInt("tex", m_pTexture->getSlot());
	//	m_pTexture->bind();

	//	RENDER.draw(m_vertexArray, m_indexBuffer, *m_pShaderProgram);
	//}
	
	bool Image2D::isValid() const
	{
		if(m_pTexture)
			return true;
		return false;
	}

	void Image2D::init()
	{
		constexpr GLfloat vertex_coords[] =
		{
			//! ������ ������� �� ���� �������������, ����� ���������� �� ���� ��������
			//1 - 2
			//|   |
			//0 - 3

			//! X and Y
			0.f, 0.f,
			0.f, 1.f,
			1.f, 1.f,
			1.f, 0.f,
		};
		const GLfloat texture_coords[] =
		{
			//! U and V
			m_subTexture.leftBottomUV.x, m_subTexture.leftBottomUV.y,
			m_subTexture.leftBottomUV.x, m_subTexture.rightTopUV.y,
			m_subTexture.rightTopUV.x, m_subTexture.rightTopUV.y,
			m_subTexture.rightTopUV.x, m_subTexture.leftBottomUV.y,
		};

		//! ������� ������������ ����� (������ � ������� ���������) ��� ���� �������������
		constexpr GLuint indexes[] =
		{
			0, 1, 2,
			2, 3, 0
		};

		m_vertexCoordsBuffer.init(&vertex_coords, sizeof(vertex_coords));
		VertexBufferLayout vertexCoordsLayout;
		vertexCoordsLayout.addElementLayoutFloat(2, false);
		m_vertexArray.addBuffer(m_vertexCoordsBuffer, vertexCoordsLayout);

		m_textureCoordsBuffer.init(&texture_coords, sizeof(texture_coords));
		VertexBufferLayout vertexTextureCoordsLayout;
		vertexTextureCoordsLayout.addElementLayoutFloat(2, false);
		m_vertexArray.addBuffer(m_textureCoordsBuffer, vertexTextureCoordsLayout);

		m_indexBuffer.init(&indexes, 6);
	}
}
