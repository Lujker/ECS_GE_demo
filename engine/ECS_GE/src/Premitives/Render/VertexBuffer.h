#pragma once
#include "glad/glad.h"

namespace RenderEngine
{
	class VertexBuffer
	{
	public:
		VertexBuffer();
		~VertexBuffer();
		VertexBuffer(VertexBuffer&&) noexcept;
		VertexBuffer& operator=(VertexBuffer&&) noexcept;

		void init(const void* data, const unsigned int data_size);
		void destroy();
		void update(const void* data, const unsigned int data_size) const;
		void bind() const;
		void unbind() const;
	private:

		VertexBuffer(const VertexBuffer&) = delete;
		VertexBuffer& operator=(const VertexBuffer&) = delete;

		GLuint m_id;
	};
}
