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
		m_characters.clear();
		initChars();
		initial = true;
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

void DisplayString::initChars()
{
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Disable byte-alignment restriction
    m_characters.clear();
    for (const auto& c : m_initString)
    {
        // Load character glyph
        if (FT_Load_Char(FONT.getFace(), c, FT_LOAD_RENDER))
        {
            std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
            continue;
        }
        // Now store character for later use
		const auto gluph = FONT.getFace()->glyph;
        Character character = {
            std::make_shared<RenderEngine::Texture2D>(gluph->bitmap.width, gluph->bitmap.rows, gluph->bitmap.buffer, 1),
            glm::ivec2(gluph->bitmap_left, gluph->bitmap_top),
			gluph->advance.x
        };
        m_characters.insert(std::pair(c, character));
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
		0.f, 0.f,
		0.f, -1.f,
		1.f, -1.f,
		1.f, 0.f,
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
