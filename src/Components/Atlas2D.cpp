#include "Atlas2D.h"
namespace RenderEngine
{
	TextureAtlas::TextureAtlas(const GLuint width, GLuint heigh, 
		const unsigned char* data, const unsigned int channels, 
		const GLenum filter, const GLenum wrapMode, 
		const unsigned int textureSlot) :
	Texture2D(width, heigh, data, channels, filter, wrapMode, textureSlot)
	{}

	const SubTexture2D& TextureAtlas::addSubTexture(std::string name, const glm::vec2& leftBottomUV, const glm::vec2& rightTopUV)
	{
		auto it = m_subTexture.emplace(std::move(name), SubTexture2D(leftBottomUV, rightTopUV));
		if (it.second)
			return it.first->second;
		const static  SubTexture2D def_texture;
		return def_texture;
	}

	const SubTexture2D& TextureAtlas::getSubTexture(std::string name)
	{
		auto it = m_subTexture.find(name);
		if (it != m_subTexture.end())
		{
			return it->second;
		}
		const static SubTexture2D def_texture;
		return def_texture;
	}
}