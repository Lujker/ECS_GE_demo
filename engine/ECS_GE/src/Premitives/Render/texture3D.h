#pragma once 
#include <vector>
#include <string>
#include "gl_Include.h"

namespace RenderEngine
{
	enum class eImageFormat : unsigned
	{
		eNone = 0,
		ePNG = 1,
		eJPG = 2,
		eJPEG = 3
	};
	
	std::string getAdditionalStringByFormat(const eImageFormat&);

	class Texture3D
	{
	public:
		explicit Texture3D(const std::vector<std::string>& faces,
			const unsigned int channels = 4,
			const GLenum filter = GL_LINEAR,
			const GLenum wrapMode = GL_CLAMP_TO_EDGE);

		explicit Texture3D(const std::string& faces_directory, eImageFormat format,
			const unsigned int channels = 4,
			const GLenum filter = GL_LINEAR,
			const GLenum wrapMode = GL_CLAMP_TO_EDGE);

		void bind(int iTextureUnit = 0) const;
		const unsigned getWidth()
		{
			return m_width;
		}
		const unsigned getHeight()
		{
			return m_height;
		}
		const std::vector<std::string>& getTexturePath()
		{
			return m_faces;
		}
		bool isInit()
		{
			return is_init;
		}
		virtual ~Texture3D();
	private:
		void init(
			const unsigned int channels = 4,
			const GLenum filter = GL_LINEAR,
			const GLenum wrapMode = GL_CLAMP_TO_EDGE);
		GLuint m_ID;
		GLenum m_mode;
		bool is_init = false;
		int m_width = 0, m_height = 0, m_Channels = 4;
		std::vector<std::string> m_faces;
	};
}