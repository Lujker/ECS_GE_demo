#include "Sphere.h"
#include <iostream>
#include <math.h>
#include <vector>
#include "texture2D.h"

namespace RenderEngine 
{
	GLfloat PI = 3.14159265358979323846f;

	double sqr(double x) { return x * x; }

	void CalcNormals(Vector3 a, Vector3 b, Vector3 c, Vector3& n)
	{
		double wrki;
		Vector3 v1, v2;

		v1.x = a.x - b.x;
		v1.y = a.y - b.y;
		v1.z = a.z - b.z;

		v2.x = b.x - c.x;
		v2.y = b.y - c.y;
		v2.z = b.z - c.z;

		wrki = sqrt(sqr(v1.y * v2.z - v1.z * v2.y) + sqr(v1.z * v2.x - v1.x * v2.z) + sqr(v1.x * v2.y - v1.y * v2.x));
		n.x = (v1.y * v2.z - v1.z * v2.y) / wrki;
		n.y = (v1.z * v2.x - v1.x * v2.z) / wrki;
		n.z = (v1.x * v2.y - v1.y * v2.x) / wrki;
	}

	Sphere::Sphere()
	{
		init();
	}

	Sphere::Sphere(const std::shared_ptr<Texture2D>& pTexture, std::shared_ptr<Texture2D> specularMap)
	{
		if (pTexture)
		{
			m_material.m_pTexture = pTexture;
			m_material.m_pTexture->setSlot(0);
		}
		if (specularMap) 
		{
			m_material.m_specularMap = specularMap;
			m_material.m_specularMap->setSlot(1);
		}
		init();
	}
	Sphere::Sphere(const Sphere& sphere) : m_material(sphere.m_material)
	{
		init();
	}
	Sphere& Sphere::operator=(const Sphere& sphere)
	{
		m_material = sphere.m_material;
		init();
		return *this;
	}
	Sphere::Sphere(Sphere&& sphere) noexcept :
		m_material(std::move(sphere.m_material)),
		m_vertexArray(std::move(sphere.m_vertexArray)),
		m_vertexCoordsBuffer(std::move(sphere.m_vertexCoordsBuffer)),
		m_indexCoordsBuffer(std::move(sphere.m_indexCoordsBuffer))
	{
	}
	Sphere& Sphere::operator=(Sphere&& sphere) noexcept
	{
		m_material = std::move(sphere.m_material);
		m_vertexArray = std::move(sphere.m_vertexArray);
		m_vertexCoordsBuffer = std::move(sphere.m_vertexCoordsBuffer);
		m_indexCoordsBuffer = std::move(sphere.m_indexCoordsBuffer);
		return *this;
	}
	void Sphere::SetSpecularMap(const std::shared_ptr<Texture2D>& specularMap)
	{
		m_material.m_specularMap = specularMap;
	}
	void Sphere::SetDiffuseTexture(const std::shared_ptr<Texture2D>& diffuseTexture)
	{
		m_material.m_specularMap = diffuseTexture;
	}
	void Sphere::SetShininess(float shininess)
	{
		m_material.shininess = shininess;
	}
	void Sphere::updateBuffer(const void* data, const unsigned int data_size)
	{
		m_vertexCoordsBuffer.update(data, data_size);
	}
	void Sphere::init()
	{
		std::vector<GLfloat> sphereVertices;
		std::vector<GLuint> sphereIndices;
		const int Y_SEGMENTS = 50;
		const int X_SEGMENTS = 50;
		int vertexCount = 0;
		int normalsCount = 0;
		int textureCount = 0;
		// *2 - Вычислить вершины сферы * /
			// Создаем вершины шара
		for (int y = 0; y <= Y_SEGMENTS; y++)
		{
			for (int x = 0; x <= X_SEGMENTS; x++)
			{
				float xSegment = (float)x / (float)X_SEGMENTS;
				float ySegment = (float)y / (float)Y_SEGMENTS;
				float xPos = std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
				float yPos = std::cos(ySegment * PI);
				float zPos = std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
				sphereVertices.push_back(xPos);
				sphereVertices.push_back(yPos);
				sphereVertices.push_back(zPos);
			}
		}
		vertexCount = sphereVertices.size();
		// Вычисляем нормали
		for (int pos = 0; pos <= Y_SEGMENTS * X_SEGMENTS; pos += 9)
		{
			Vector3 n;
			CalcNormals({ sphereVertices.at(pos + 0), sphereVertices.at(pos + 1), sphereVertices.at(pos + 2) },
						{ sphereVertices.at(pos + 3), sphereVertices.at(pos + 4), sphereVertices.at(pos + 5) },
						{ sphereVertices.at(pos + 6), sphereVertices.at(pos + 7), sphereVertices.at(pos + 8) }, n);
			for (int i = 0; i < 3; ++i)
			{
				sphereVertices.push_back(n.x);
				sphereVertices.push_back(n.y);
				sphereVertices.push_back(n.z);
			}
		}
		normalsCount = sphereVertices.size() - vertexCount;

		for (int y = 0; y <= Y_SEGMENTS; y++)
		{
			for (int x = 0; x <= X_SEGMENTS; x++)
			{
				float u = 1.0f - float(x) / Y_SEGMENTS;
				float v = 1.0f - float(y) / X_SEGMENTS;
				sphereVertices.push_back(u);
				sphereVertices.push_back(v);
			}
		}
		textureCount = sphereVertices.size() - vertexCount - normalsCount;

		// Индексы, генерирующие мяч
		for (int i = 0; i < Y_SEGMENTS; i++)
		{
			for (int j = 0; j < X_SEGMENTS; j++)
			{
				sphereIndices.push_back(i * (X_SEGMENTS + 1) + j + 1);
				sphereIndices.push_back((i + 1) * (X_SEGMENTS + 1) + j + 1);
				sphereIndices.push_back(i * (X_SEGMENTS + 1) + j);
				sphereIndices.push_back((i + 1) * (X_SEGMENTS + 1) + j + 1);
				sphereIndices.push_back((i + 1) * (X_SEGMENTS + 1) + j);
				sphereIndices.push_back(i * (X_SEGMENTS + 1) + j);
			}
		}

		m_vertexCoordsBuffer.init(sphereVertices.data(), vertexCount * sizeof(GLfloat));
		VertexBufferLayout vertexCoordsLayout;
		vertexCoordsLayout.addElementLayoutFloat(3, false);
		m_vertexArray.addBuffer(m_vertexCoordsBuffer, vertexCoordsLayout);

		m_normalsCoordsBuffer.init(sphereVertices.data() + vertexCount, normalsCount * sizeof(GLfloat));
		VertexBufferLayout vertexNormalCoordsLayout;
		vertexNormalCoordsLayout.addElementLayoutFloat(3, false);
		m_vertexArray.addBuffer(m_normalsCoordsBuffer, vertexNormalCoordsLayout);

		m_texturesCoordsBuffer.init(sphereVertices.data() + vertexCount + normalsCount, textureCount * sizeof(GLfloat));
		VertexBufferLayout vertexTextureCoordsLayout;
		vertexTextureCoordsLayout.addElementLayoutFloat(2, false);
		m_vertexArray.addBuffer(m_texturesCoordsBuffer, vertexTextureCoordsLayout);

		m_indexCoordsBuffer.init(sphereIndices.data(), sphereIndices.size());
	}
}