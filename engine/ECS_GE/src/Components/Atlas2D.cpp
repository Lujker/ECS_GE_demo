#include "Atlas2D.h"

namespace RenderEngine
{
	SpriteAtlas::SpriteAtlas(const GLuint width, GLuint heigh, 
		const unsigned char* data, const unsigned int channels, 
		const GLenum filter, const GLenum wrapMode, 
		const unsigned int textureSlot) :
	Texture2D(width, heigh, data, channels, filter, wrapMode, textureSlot)
	{}

	//const SubTexture2D& SpriteAtlas::addSubTexture(std::string name, const glm::vec2& leftBottomUV, const glm::vec2& rightTopUV)
	//{
	//	auto it = m_subTextures.emplace(std::move(name), SubTexture2D(leftBottomUV, rightTopUV));
	//	if (it.second)
	//		return it.first->second;
	//	const static SubTexture2D def_texture;
	//	return def_texture;
	//}

	void SpriteAtlas::addAnimation(std::string name, const animations_frame& anim)
	{
		m_Animations.emplace(std::move(name), anim);
	}

	const SpriteAtlas::animations_frame& SpriteAtlas::getAnimation(std::string name)
	{
		const auto it = m_Animations.find(name);
		if (it != m_Animations.end())
		{
			return it->second;
		}
		const static SpriteAtlas::animations_frame def_anim;
		return def_anim;
	}

	FrameAtlas::FrameAtlas(const GLuint width, GLuint heigh, 
		const unsigned char* data, const unsigned int channels, 
		const GLenum filter, const GLenum wrapMode, 
		const unsigned int textureSlot) :
		Texture2D(width, heigh, data, channels, filter, wrapMode, textureSlot)
	{
	}
	void FrameAtlas::addFrame(std::string name, const SubTexture2D& sub_texture)
	{
		m_frames.emplace(std::move(name), sub_texture);
	}
	const std::map<std::string, SubTexture2D>& FrameAtlas::getFrames() const
	{
		return m_frames;
	}
	std::list<std::string> FrameAtlas::getNames() const
	{
		std::list<std::string> lst;
		for (const auto& it : m_frames)
			lst.push_back(it.first);
		return lst;
	}
	FrameAtlas::frames_iterator FrameAtlas::getFrame(std::string name)
	{
		return m_frames.find(name);
	}
}
