#include "DisplayString.h"

#include <iostream>

DisplayString::DisplayString(const std::string& string, unsigned int width, unsigned int height):
	m_initString(string), width(width), height(height)
{
	if (FONT.isInit())
	{
		FT_Set_Pixel_Sizes(FONT.getFace(), width, height);
		init();
		initial = true;
	}
	else
		initial = false;
}

DisplayString::~DisplayString()
{
	clearString();
}

void DisplayString::setString(const std::string& string, unsigned int width, unsigned int height)
{
	if (FONT.isInit())
	{
		FT_Set_Pixel_Sizes(FONT.getFace(), width, height);
		m_initString = string;
		initChars();
		initial = true;
	}
}

void DisplayString::updateString(const std::string& string)
{
	if (FONT.isInit())
	{
		m_initString = string;
		initial = true;
		initChars(true);
	}
}

void DisplayString::clearString()
{
	m_characters.clear();
	m_initString.clear();
	initial = false;
}

void DisplayString::resize(unsigned int width, unsigned int height)
{
	FT_Set_Pixel_Sizes(FONT.getFace(), width, height);
	m_characters.clear();
	initChars();
}

bool DisplayString::isEmpty() const
{
	return !bufferInitioal && !initial && !m_characters.empty();
}

std::list<Character> DisplayString::getDisplayChars()
{
	std::list<Character> list;
	if (initial && !m_characters.empty())
	{
		for(const auto& it : m_initString)
		{
			auto ch = m_characters.find(it);
			if (ch != m_characters.end())
				list.emplace_back(ch->second);
		}
	}
	return list;
}

const Rect DisplayString::getRect(float scale)
{
	auto charList = getDisplayChars();
	unsigned width = 0, height = 0;
	float x = 0, y = 0;
	for (const auto& ch : charList)
	{
		const float y_offset = (ch.Bearing.y - static_cast<float>(ch.texture->getHeight()));
		const float x_offset = ch.Bearing.x * scale;
		x += x_offset;
		if ((ch.texture->getHeight() - y_offset) * scale  > height)
		{
			height = (ch.texture->getHeight() - y_offset) * scale;
			y = y_offset * scale;
		}
		x += (ch.Advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64)
	}
	width = x;
	return Rect(0, y, width, height);
}

void DisplayString::mirror(bool vertical, bool horizontal)
{
	const GLfloat vertex_coords[] =
	{
		//! X and Y
		horizontal ? 1.f : 0.f, vertical ? 0.f : 1.f,
		horizontal ? 1.f : 0.f, vertical ? 1.f : 0.f,
		horizontal ? 0.f : 1.f, vertical ? 1.f : 0.f,
		horizontal ? 0.f : 1.f, vertical ? 0.f : 1.f,
	};
	m_vertexCoordsBuffer.update(vertex_coords, sizeof(vertex_coords));
}

void DisplayString::initChars(bool update)
{
	if(!update)
		m_characters.clear();
    for (const auto& c : m_initString)
    {
		auto curr_char = FONT.getCharacter(c);
		if(curr_char.has_value())
			m_characters.insert(std::pair(c, *curr_char));
    }
}

void DisplayString::initBuffers()
{
	constexpr GLfloat vertex_coords[] =
	{
		//! делаем квадрат из двух треугольников, затем натягиваем на него текстуру
		//1 - 2
		//|   |
		//0 - 3

		//! X and Y
		0.f, 1.f,
		0.f, 0.f,
		1.f, 0.f,
		1.f, 1.f,
	};
	const GLfloat texture_coords[] =
	{
		//! U and V
		0.f, 0.f,
		0.f, 1.f,
		1.f, 1.f,
		1.f, 0.f,
	};

	//! индексы расположений точек (индекс в массиве вертексов) для двух треугольников
	constexpr GLuint indexes[] =
	{
		0, 1, 2,
		2, 3, 0
	};

	m_vertexCoordsBuffer.init(&vertex_coords, sizeof(vertex_coords));
	RenderEngine::VertexBufferLayout vertexCoordsLayout;
	vertexCoordsLayout.addElementLayoutFloat(2, false);
	m_vertexArray.addBuffer(m_vertexCoordsBuffer, vertexCoordsLayout);

	m_textureCoordsBuffer.init(&texture_coords, sizeof(texture_coords));
	RenderEngine::VertexBufferLayout vertexTextureCoordsLayout;
	vertexTextureCoordsLayout.addElementLayoutFloat(2, false);
	m_vertexArray.addBuffer(m_textureCoordsBuffer, vertexTextureCoordsLayout);

	m_indexBuffer.init(&indexes, 6);
	bufferInitioal = true;
}

void DisplayString::init()
{
	initBuffers();
	initChars();
}
