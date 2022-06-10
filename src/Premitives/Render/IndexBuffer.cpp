#include "IndexBuffer.h"
namespace RenderEngine
{
	IndexBuffer::IndexBuffer() :
		m_id(0),
		countElemets(0)
	{
	}

	IndexBuffer::~IndexBuffer()
	{
		if (m_id)
			glDeleteBuffers(1, &m_id);
	}

	IndexBuffer::IndexBuffer(IndexBuffer&& buffer) noexcept :
		m_id(buffer.m_id), countElemets(buffer.countElemets)
	{
		buffer.m_id = 0;
		buffer.countElemets = 0;
	}

	IndexBuffer& IndexBuffer::operator=(IndexBuffer&& buffer) noexcept
	{
		m_id = buffer.m_id;
		buffer.m_id = 0;
		countElemets = buffer.countElemets;
		buffer.countElemets = 0;
		return *this;
	}

	void IndexBuffer::init(const void* data, const unsigned int count)
	{
		glGenBuffers(1, &m_id);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * count, data, GL_STATIC_DRAW);
		countElemets = count;
	}

	void IndexBuffer::bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
	}

	void IndexBuffer::unbind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}