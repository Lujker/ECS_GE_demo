#pragma once

#include <map>
#include <string>
#include <glad/glad.h>
#include <glm/vec2.hpp>

namespace RenderEngine
{
	struct FrameDescription;

	class Texture2D
	{
	public:
		Texture2D(	const std::string& path, 
					const unsigned int channels, 
					const std::string& type = "texture_diffuse",
					const unsigned int textureSlot = GL_TEXTURE0,
					const GLenum filter = GL_LINEAR, 
					const GLenum wrapMode = GL_CLAMP_TO_EDGE);

		Texture2D(	const GLuint width, GLuint heigh,
					const unsigned char* data,
					const unsigned int channels,
					const std::string& type = "texture_diffuse",
					const unsigned int textureSlot = GL_TEXTURE0,
					const GLenum filter = GL_LINEAR,
					const GLenum wrapMode = GL_CLAMP_TO_EDGE);

		Texture2D(Texture2D&& texture2D) noexcept;
		Texture2D& operator = (Texture2D&& texture2D) noexcept;
		void bind(int iTextureUnit = 0) const;
		void setSlot(unsigned slot);
		const unsigned getWidth()
		{
			return m_width;
		}
		const unsigned getHeight()
		{
			return m_height;
		}
		const int getSlot()
		{
			return m_slot - GL_TEXTURE0;
		}
		std::string getTextureType() 
		{
			return m_type;
		}
		std::string getTexturePath()
		{
			return m_path;
		}
		bool isInit() 
		{
			return is_init;
		}
		virtual ~Texture2D();
	protected:
		Texture2D() = delete;
		Texture2D(const Texture2D&) = delete;
		Texture2D& operator= (const Texture2D&) = delete;

		GLuint m_ID;
		GLenum m_mode;
		unsigned int m_width;
		unsigned int m_height;
		int m_slot;
		std::string m_type;
		std::string m_path;
		bool is_init = false;
	};
}
