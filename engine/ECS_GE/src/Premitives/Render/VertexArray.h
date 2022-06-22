#pragma once
#include "glad/glad.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

namespace RenderEngine
{
	class VertexArray
	{
	public:
		VertexArray();
		~VertexArray();
		VertexArray(VertexArray&&) noexcept;
		VertexArray& operator=(VertexArray&&) noexcept;
		
		void addBuffer(const VertexBuffer& vertexBuffer, const VertexBufferLayout& layuot);
		void bind() const;
		void unbind() const;
	private:

		VertexArray(const VertexArray&) = delete;
		VertexArray& operator=(const VertexArray&) = delete;

		GLuint m_id = 0;
		size_t m_elementsCount = 0;
	};
}
