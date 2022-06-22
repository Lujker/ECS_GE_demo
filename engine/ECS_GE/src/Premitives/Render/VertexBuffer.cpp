#include "VertexBuffer.h"
namespace RenderEngine
{
	VertexBuffer::VertexBuffer() :
		m_id(0)
	{
	}

	VertexBuffer::~VertexBuffer()
	{
		if (m_id)
			glDeleteBuffers(1, &m_id);
	}

	VertexBuffer::VertexBuffer(VertexBuffer&& buffer) noexcept
	{
		m_id = buffer.m_id;
		buffer.m_id = 0;
	}

	VertexBuffer& VertexBuffer::operator=(VertexBuffer&& buffer) noexcept
	{
		m_id = buffer.m_id;
		buffer.m_id = 0;
		return *this;
	}

	void VertexBuffer::init(const void* data, const unsigned int data_size)
	{
		if (m_id)
		{
			glDeleteBuffers(1, &m_id);
			m_id = 0;
		}
		glGenBuffers(1, &m_id);
		glBindBuffer(GL_ARRAY_BUFFER, m_id);
		glBufferData(GL_ARRAY_BUFFER, data_size, data, GL_STATIC_DRAW);
	}

	void VertexBuffer::destroy()
	{
		if (m_id)
			glDeleteBuffers(1, &m_id);
		m_id = 0;
	}
	
	void VertexBuffer::update(const void* data, const unsigned int data_size) const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_id);
		glBufferSubData(GL_ARRAY_BUFFER, 0, data_size, data);
	}

	void VertexBuffer::bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_id);
	}

	void VertexBuffer::unbind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}