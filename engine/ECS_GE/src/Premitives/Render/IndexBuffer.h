#pragma once
#include "glad/glad.h"

namespace RenderEngine
{
	class IndexBuffer
	{
	public:
		IndexBuffer();
		~IndexBuffer();
		IndexBuffer(IndexBuffer&&) noexcept;
		IndexBuffer& operator=(IndexBuffer&&) noexcept;

		void init(const void* data, const unsigned int count);
		void bind() const;
		void unbind() const;
		const unsigned int getCountElements() const { return countElemets; }
	private:

		IndexBuffer(const IndexBuffer&) = delete;
		IndexBuffer& operator=(const IndexBuffer&) = delete;

		GLuint m_id;
		unsigned int countElemets = 0;
	};
}
