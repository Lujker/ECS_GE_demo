#pragma once
#include "glad/glad.h"
#include <vector>


namespace RenderEngine
{
	struct VertexBufferLayoutElement
	{
		GLint count;
		GLenum type;
		GLboolean normalized;
		unsigned long long size;
	};

	class VertexBufferLayout
	{
	public:
		VertexBufferLayout();
		~VertexBufferLayout();

		void reservElements(const size_t count);
		unsigned int getStride() const { return m_stride; }
		const std::vector<VertexBufferLayoutElement>& getLayoutElements() const { return m_layoutElements; }
		void addElementLayoutFloat(const int count, const bool normalized);

	private:
		std::vector<VertexBufferLayoutElement> m_layoutElements;
		unsigned m_stride;
	};
}
