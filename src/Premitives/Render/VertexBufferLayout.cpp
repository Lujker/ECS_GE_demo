#include "VertexBufferLayout.h"
namespace RenderEngine
{
	VertexBufferLayout::VertexBufferLayout() :
		m_stride(0)
	{}

	VertexBufferLayout::~VertexBufferLayout()
	{
	}

	void VertexBufferLayout::reservElements(const size_t count)
	{
		m_layoutElements.reserve(count);
	}

	void VertexBufferLayout::addElementLayoutFloat(const int count, const bool normalized)
	{
		m_layoutElements.push_back(VertexBufferLayoutElement{ count , GL_FLOAT, normalized, count * sizeof(GLfloat)});
		m_stride += m_layoutElements.back().size;
	}
}