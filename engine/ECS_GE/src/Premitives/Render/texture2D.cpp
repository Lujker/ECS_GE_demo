#include "texture2D.h"
#include <gl/GL.h>
#include "Magick++.h"
#include "Engine.h"
#include "LogSystem.h"

RenderEngine::Texture2D::Texture2D(const std::string& path,
	const unsigned int channels,
	const std::string& type,
	const unsigned int textureSlot,
	const GLenum filter, 
	const GLenum wrapMode): m_slot(textureSlot), m_type(type), m_path(path)
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
	std::string filename = std::string(path);
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
	//!���������� �������� ��� ����� id � ������ �� �������� � opengl ������
	glGenTextures(1, &m_ID);
	//!��������� ���� � ������ ������ ���� ��������
	glActiveTexture(m_slot);
	bind();
	//���������� ������ ��� �������� ��������
	glTexImage2D(GL_TEXTURE_2D, 0, m_mode, pImage.columns(), pImage.rows(), 0, m_mode, GL_UNSIGNED_BYTE, blob.data());
	//! wrapMode ��������� �������� ��� ������ ��������� ���������� �� ��������� ������� �������� ��������
	//!CLAMP_TO_EDGE ��������� ��������� ������� �������� �� �������� ��������� �� �� �������
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);
	//! min_filter �������� ���������� ����� �������� � �������� ������ ��� ����� ��� ��������� ��������
	//! �������� �� ����������� ������� ������������� ������� �������� ����� ��������� ��� GL_LINEAR
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
	//! max_filter ���� �������� ��� ��������� ��� ������� ��������� ����� �������� ���������� � ��������� ��������
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
	//! ���������� ��� ������ minmap
	glGenerateMipmap(GL_TEXTURE_2D);
	//������� ������� ���� �������� �� ����� ��������
	glBindTexture(GL_TEXTURE_2D, 0);
	is_init = true;
}

RenderEngine::Texture2D::Texture2D(const GLuint width, GLuint heigh,
	const unsigned char* data,
	const unsigned int channels,
	const std::string& type,
	const unsigned int textureSlot,
	const GLenum filter,
	const GLenum wrapMode) :
	m_width(width), m_height(heigh), m_slot(textureSlot), m_type(type)
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

	//!���������� �������� ��� ����� id � ������ �� �������� � opengl ������
	glGenTextures(1, &m_ID);
	//!��������� ���� � ������ ������ ���� ��������
	glActiveTexture(m_slot);
	bind();
	//���������� ������ ��� �������� ��������
	glTexImage2D(GL_TEXTURE_2D, 0, m_mode, m_width, m_height, 0, m_mode, GL_UNSIGNED_BYTE, data);
	//! wrapMode ��������� �������� ��� ������ ��������� ���������� �� ��������� ������� �������� ��������
	//!CLAMP_TO_EDGE ��������� ��������� ������� �������� �� �������� ��������� �� �� �������
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);
	//! min_filter �������� ���������� ����� �������� � �������� ������ ��� ����� ��� ��������� ��������
	//! �������� �� ����������� ������� ������������� ������� �������� ����� ��������� ��� GL_LINEAR
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
	//! max_filter ���� �������� ��� ��������� ��� ������� ��������� ����� �������� ���������� � ��������� ��������
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
	//! ���������� ��� ������ minmap
	glGenerateMipmap(GL_TEXTURE_2D);
	//������� ������� ���� �������� �� ����� ��������
	glBindTexture(GL_TEXTURE_2D, 0);
	is_init = true;
}

RenderEngine::Texture2D::Texture2D(Texture2D&& texture2D) noexcept :
	m_ID(texture2D.m_ID), m_mode(texture2D.m_mode), m_width(texture2D.m_width), m_height(texture2D.m_height), m_slot(texture2D.m_slot)
{
	texture2D.m_ID = 0;
}

RenderEngine::Texture2D& RenderEngine::Texture2D::operator=(Texture2D&& texture2D) noexcept
{
	glDeleteTextures(1, &m_ID);
	m_ID = texture2D.m_ID;
	texture2D.m_ID = 0;
	m_height = texture2D.m_height;
	m_width = texture2D.m_width;
	m_mode = texture2D.m_mode;
	m_slot = texture2D.m_slot;
	return *this;
}

void RenderEngine::Texture2D::bind(int iTextureUnit) const
{
	glActiveTexture(m_slot);
	glBindTexture(GL_TEXTURE_2D, m_ID); //! ��������� �������� ��� ������ id ��� ������� �������� � ������
}

void RenderEngine::Texture2D::setSlot(unsigned slot)
{
	m_slot = GL_TEXTURE0 + slot;
}

RenderEngine::Texture2D::~Texture2D()
{
	glDeleteTextures(1, &m_ID);
}
