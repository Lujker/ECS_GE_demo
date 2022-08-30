#include "Cube.h"
namespace RenderEngine
{
	void Cube::init()
	{
		//0-2 vertex coord, 3-5 normal coords, 6-7 texture coords
		constexpr GLfloat vertex_coords[] = {
			-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,
			 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,

			-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,  0.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,  1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,  1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,  1.0f, 1.0f,
			-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,  0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,  0.0f, 0.0f,

			-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
			-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
			-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
			-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,

			 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,

			-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f,

			-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f,
			 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f
		};

		m_vertexCoordsBuffer.init(&vertex_coords, sizeof(vertex_coords));
		VertexBufferLayout vertexCoordsLayout;
		vertexCoordsLayout.addElementLayoutFloat(3, false);
		vertexCoordsLayout.addElementLayoutFloat(3, false);
		vertexCoordsLayout.addElementLayoutFloat(2, false);
		m_vertexArray.addBuffer(m_vertexCoordsBuffer, vertexCoordsLayout);
	}

	Cube::Cube(const std::shared_ptr<Texture2D>& pTexture, std::shared_ptr<Texture2D> specularMap)
	{
		m_material.m_pTexture = pTexture;
		m_material.m_pTexture->setSlot(0);
		m_material.m_specularMap = specularMap;
		m_material.m_specularMap->setSlot(1);
		init();
	}

	Cube::Cube(const Cube& cube) : m_material(cube.m_material)
	{
		init();
	}

	Cube& Cube::operator=(const Cube& cube)
	{
		m_material = cube.m_material;
		init();
		return *this;
	}

	Cube::Cube(Cube&& cube) noexcept : 
		m_material(std::move(cube.m_material)), 
		m_vertexArray(std::move(cube.m_vertexArray)), 
		m_vertexCoordsBuffer(std::move(cube.m_vertexCoordsBuffer))
	{}

	Cube& Cube::operator=(Cube&& cube) noexcept
	{
		m_material = std::move(cube.m_material);
		m_vertexArray = std::move(cube.m_vertexArray);
		m_vertexCoordsBuffer = std::move(cube.m_vertexCoordsBuffer);
		return *this;
	}

	void Cube::SetSpecularMap(const std::shared_ptr<Texture2D>& specularMap)
	{	
		m_material.m_specularMap = specularMap;
	}

	void Cube::SetDiffuseTexture(const std::shared_ptr<Texture2D>& diffuseTexture)
	{
		m_material.m_specularMap = diffuseTexture;
	}

	void Cube::SetShininess(float shininess)
	{
		m_material.shininess = shininess;
	}

	void Cube::updateBuffer(const void* data, const unsigned int data_size)
	{
		m_vertexCoordsBuffer.update(data, data_size);
	}

	LightCube::LightCube(const ColorComponent& color):
		m_color(color)
	{
		init();
	}

	LightCube::LightCube(LightCube&& cube) noexcept :
		m_vertexArray(std::move(cube.m_vertexArray)), m_vertexCoordsBuffer(std::move(cube.m_vertexCoordsBuffer)), m_color(std::move(cube.m_color))
	{}

	LightCube& LightCube::operator=(LightCube&& cube) noexcept
	{
		m_vertexArray = std::move(cube.m_vertexArray);
		m_vertexCoordsBuffer = std::move(cube.m_vertexCoordsBuffer);
		return *this;
	}

	ColorComponent LightCube::setColor(const ColorComponent& color)
	{
		auto before = m_color;
		m_color = color;
		return before;
	}

	void LightCube::init()
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

		m_vertexCoordsBuffer.init(&vertex_coords, sizeof(vertex_coords));
		VertexBufferLayout vertexCoordsLayout;
		vertexCoordsLayout.addElementLayoutFloat(3, false);
		m_vertexArray.addBuffer(m_vertexCoordsBuffer, vertexCoordsLayout);
	}
}
