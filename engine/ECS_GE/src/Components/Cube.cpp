#include "Cube.h"
namespace RenderEngine
{
	void Cube::init()
	{
		constexpr GLfloat vertex_coords[] = {
		-0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,

		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,

		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,

		-0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f
		};

		constexpr GLfloat texture_coords[] =
		{
			 0.0f, 0.0f,
			 1.0f, 0.0f,
			 1.0f, 1.0f,
			 1.0f, 1.0f,
			 0.0f, 1.0f,
			 0.0f, 0.0f,

			 0.0f, 0.0f,
			 1.0f, 0.0f,
			 1.0f, 1.0f,
			 1.0f, 1.0f,
			 0.0f, 1.0f,
			 0.0f, 0.0f,

			 1.0f, 0.0f,
			 1.0f, 1.0f,
			 0.0f, 1.0f,
			 0.0f, 1.0f,
			 0.0f, 0.0f,
			 1.0f, 0.0f,

			 1.0f, 0.0f,
			 1.0f, 1.0f,
			 0.0f, 1.0f,
			 0.0f, 1.0f,
			 0.0f, 0.0f,
			 1.0f, 0.0f,

			 0.0f, 1.0f,
			 1.0f, 1.0f,
			 1.0f, 0.0f,
			 1.0f, 0.0f,
			 0.0f, 0.0f,
			 0.0f, 1.0f,

			 0.0f, 1.0f,
			 1.0f, 1.0f,
			 1.0f, 0.0f,
			 1.0f, 0.0f,
			 0.0f, 0.0f,
			 0.0f, 1.0f
		};


		m_vertexCoordsBuffer.init(&vertex_coords, sizeof(vertex_coords));
		VertexBufferLayout vertexCoordsLayout;
		vertexCoordsLayout.addElementLayoutFloat(3, false);
		m_vertexArray.addBuffer(m_vertexCoordsBuffer, vertexCoordsLayout);

		m_textureCoordsBuffer.init(&texture_coords, sizeof(texture_coords));
		VertexBufferLayout vertexTextureCoordsLayout;
		vertexTextureCoordsLayout.addElementLayoutFloat(2, false);
		m_vertexArray.addBuffer(m_textureCoordsBuffer, vertexTextureCoordsLayout);
	}

	Cube::Cube(const std::shared_ptr<Texture2D>& pTexture)
	{
		m_pTexture = pTexture;
		m_subTexture = SubTexture2D();
		init();
	}
	Cube::Cube(const std::shared_ptr<SpriteAtlas>& pAtlas, const SubTexture2D& sub_texture)
	{
		m_pTexture = pAtlas;
		m_subTexture = sub_texture;
		init();
	}
	Cube::Cube(const Cube& cube) : Image2D(cube)
	{}
	Cube& Cube::operator=(const Cube& cube)
	{
		m_pTexture = cube.m_pTexture;
		m_subTexture = cube.m_subTexture;
		init();
		return *this;
	}
	Cube::Cube(Cube&& cube) noexcept : Image2D(std::move(cube))
	{}
	Cube& Cube::operator=(Cube&& cube) noexcept
	{
		m_pTexture = std::move(cube.m_pTexture);
		m_subTexture = cube.m_subTexture;
		m_vertexArray = std::move(cube.m_vertexArray);
		m_vertexCoordsBuffer = std::move(cube.m_vertexCoordsBuffer);
		m_textureCoordsBuffer = std::move(cube.m_textureCoordsBuffer);
		m_indexBuffer = std::move(cube.m_indexBuffer);
		return *this;
	}
	void Cube::SetSubTexture(const SubTexture2D& sub_texture)
	{
		if (m_subTexture == sub_texture)
			return;
		m_subTexture = sub_texture;
		//const GLfloat texture_coords[] =
		//{
		//	//! U and V
		//	m_subTexture.leftBottomUV.x, m_subTexture.leftBottomUV.y,
		//	m_subTexture.leftBottomUV.x, m_subTexture.rightTopUV.y,
		//	m_subTexture.rightTopUV.x, m_subTexture.rightTopUV.y,
		//	m_subTexture.rightTopUV.x, m_subTexture.leftBottomUV.y,
		//};
		//m_textureCoordsBuffer.update(&texture_coords, sizeof(texture_coords));
	}
	bool Cube::isMirrored() const
	{
		return false;
	}
	void Cube::mirror(bool vertical, bool horizontal)
	{
		return;
	}
	void Cube::updateVertex(const void* data, const unsigned int data_size)
	{
		m_vertexCoordsBuffer.update(data, data_size);
	}
}
