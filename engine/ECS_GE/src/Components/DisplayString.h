#pragma once
#include <memory>

#include "Atlas2D.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "FontManager.h"
#include "Rect.h"

struct Character {
	std::shared_ptr<RenderEngine::Texture2D> texture; // текстура глифа
	glm::ivec2 Bearing;   // Смещение верхней левой точки глифа
	FT_Pos     Advance;   // Горизонтальное смещение до начала следующего глифа
};
class DisplayString
{
public:
	DisplayString(const std::string& string, unsigned int width = 0, unsigned int height = 48);
	~DisplayString();
	void setString(const std::string& string, unsigned int width = 0, unsigned int height = 48);
	void clearString();
	void resize(unsigned int width, unsigned int height);
	bool isEmpty() const;
	std::list<Character> getDisplayChars();
	std::string getInitString() { return m_initString; }

	const RenderEngine::VertexArray& getVertexArray() const { return m_vertexArray; }
	const RenderEngine::VertexBuffer& getVertexCoordsBuffer() const { return m_vertexCoordsBuffer; }
	const RenderEngine::VertexBuffer& getTextureCoordsBuffer() const { return m_textureCoordsBuffer; }
	const RenderEngine::IndexBuffer& getIndexCoordsBuffer() const { return m_indexBuffer; }
	const Rect						 getRect(float scale = 1.f);
	const unsigned getWidth() const { return width; }
	const unsigned getHeight() const { return height; }
	virtual void mirror(bool vertical, bool horizontal);
private:
	void initChars();
	void initBuffers();
	void init();
	std::string m_initString;
	std::map<GLchar, Character> m_characters;
	RenderEngine::VertexArray  m_vertexArray;
	RenderEngine::VertexBuffer m_vertexCoordsBuffer;
	RenderEngine::VertexBuffer m_textureCoordsBuffer;
	RenderEngine::IndexBuffer  m_indexBuffer;
	unsigned width = 0, height = 48;
	bool initial = false;
	bool bufferInitioal = false;
};
