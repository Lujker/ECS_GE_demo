#include "texture2D.h"
#include <gl/GL.h>

RenderEngine::Texture2D::Texture2D(const GLuint width,	GLuint heigh, 
	const unsigned char* data, 
	const unsigned int channels,
	const GLenum filter, 
	const GLenum wrapMode,
	const unsigned int textureSlot):
	m_width(width), m_height(heigh), m_slot(textureSlot)
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

	//!генерируем текстуру под новым id и делаем ее активным в opengl движке
	glGenTextures(1, &m_ID);
	//!Указываем слот в котром должна быть текстура
	glActiveTexture(m_slot);
	bind();
	//загружаеми данные для активной тектсуры
	glTexImage2D(GL_TEXTURE_2D, 0, m_mode, m_width, m_height, 0, m_mode, GL_UNSIGNED_BYTE, data);
	//! wrapMode указывает параметр как именно заполнять координаты за гранциами размера текстуры например
	//!CLAMP_TO_EDGE повторяет последний пиксель текстуры на гранциах выходящих за ее размеры
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);
	//! min_filter выбиоает фильтрацию когда пикселей в текстуре меньше чем нужно для указанных размеров
	//! например на отсутвующие пиксели накладывается среднее значение между соседними при GL_LINEAR
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
	//! max_filter таже проблема при отдалении при большом отдалении нужно избегать артефактов и наложения пикселей
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
	//! генерируем все уровни minmap
	glGenerateMipmap(GL_TEXTURE_2D);
	//Очищаем текущий слот текстуры от нашей текстуры
	glBindTexture(GL_TEXTURE_2D, 0); 
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

void RenderEngine::Texture2D::bind() const
{
	glActiveTexture(m_slot);
	glBindTexture(GL_TEXTURE_2D, m_ID); //! установка текстуры под данным id как текущую текстуру в движке
}

void RenderEngine::Texture2D::setSlot(unsigned slot)
{
	m_slot = GL_TEXTURE0 + slot;
}

RenderEngine::Texture2D::~Texture2D()
{
	glDeleteTextures(1, &m_ID);
}
