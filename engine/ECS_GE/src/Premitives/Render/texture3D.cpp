#include "texture3D.h"
#include <fstream>
#include "Engine.h"
#include "LogSystem.h"
#include "Magick++.h"

namespace RenderEngine
{
	std::string getAdditionalStringByFormat(const eImageFormat& format)
	{
		switch (format)
		{
		case eImageFormat::eJPEG:
			return ".jpeg";
		case eImageFormat::eJPG:
			return ".jpg";
		case eImageFormat::ePNG:
			return ".png";
		default:
			return "";
		}
	}

	Texture3D::Texture3D(const std::vector<std::string>& faces, const unsigned int channels, const GLenum filter, const GLenum wrapMode):
		m_faces(faces)
	{
		init(channels, filter, wrapMode);
	}

	Texture3D::Texture3D(const std::string& faces_directory, eImageFormat format, const unsigned int channels, const GLenum filter, const GLenum wrapMode)
	{
		std::vector<std::string> filenames{ "positive_x", "negative_x" , "positive_y" , "negative_y" , "positive_z" , "negative_z" };
		std::ifstream iff{};
		auto postfix = getAdditionalStringByFormat(format);
		for (const auto& it : filenames)
		{
			auto path = faces_directory + "\\" + it + postfix;
			iff.open(path);
			if (iff.is_open())
			{
				iff.close();
				m_faces.push_back(path);
			}
			else
			{
				LOG("Failed check file: " + path);
				m_faces.push_back({});
			}
		}
		
		init(channels, filter, wrapMode);
	}

	void Texture3D::bind(int iTextureUnit) const
	{
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_ID);
	}

	Texture3D::~Texture3D()
	{
		glDeleteTextures(1, &m_ID);
	}

	void Texture3D::init(const unsigned int channels, const GLenum filter, const GLenum wrapMode)
	{
		switch (channels)
		{
		case 4:
			m_mode = GL_RGBA;
			break;
		case 3:
			m_mode = GL_RGB;
			break;
		case 1:
			m_mode = GL_RED;
			break;
		default:
			m_mode = GL_RGBA;
			break;
		}
		glGenTextures(1, &m_ID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_ID);

		for (unsigned int i = 0; i < m_faces.size(); i++)
		{
			auto filename = m_faces.at(i);
			Magick::Image pImage;
			Magick::Blob blob;
			try {
				pImage.read(filename);
				pImage.write(&blob, channels == 4 ? "RGBA" : "RGB");
			}
			catch (Magick::Error& Error)
			{
				LOG(std::string("Error loading texture: " + filename + " : " + Error.what()));
			}
			m_width = pImage.columns(); m_height = pImage.rows();

			if (blob.length() > 0)
			{
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, m_mode, m_width, m_height, 0, m_mode, GL_UNSIGNED_BYTE, blob.data());
			}
		}
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	}
}
