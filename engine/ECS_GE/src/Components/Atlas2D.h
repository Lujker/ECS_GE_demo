#pragma once
#include <list>

#include "IComponent.h"
#include "texture2D.h"

namespace RenderEngine
{
	enum class eAtlasType : int
	{
		eNone = -1,
		eFrames = 0,
		eSprite
	};

	struct SubTexture2D
	{
		glm::vec2 leftBottomUV;
		glm::vec2 rightTopUV;
		float duration;
		SubTexture2D(const glm::vec2& leftBottomUV, const glm::vec2& rightTopUV, unsigned duration = 0) :
			leftBottomUV(leftBottomUV), rightTopUV(rightTopUV), duration(duration)
		{}
		SubTexture2D(const SubTexture2D& sub_texture) :
			leftBottomUV(sub_texture.leftBottomUV), rightTopUV(sub_texture.rightTopUV), duration(sub_texture.duration)
		{}
		SubTexture2D():
			leftBottomUV(0.f), rightTopUV(1.f), duration(0)
		{}
		~SubTexture2D() = default;
		bool operator==(SubTexture2D right) { return this->rightTopUV == right.rightTopUV && this->leftBottomUV == right.leftBottomUV; }
		bool operator!=(SubTexture2D right) { return !((*this) == right); }
	};

	class FrameAtlas : public Texture2D
	{
	public:
		using frames_iterator = std::map<std::string, SubTexture2D>::const_iterator;
		FrameAtlas(const std::string& path,
			const unsigned int channels, const std::string& type = "texture_diffuse",
			const unsigned int textureSlot = GL_TEXTURE0,
			const GLenum filter = GL_LINEAR,
			const GLenum wrapMode = GL_CLAMP_TO_EDGE);
		~FrameAtlas() override = default;
		virtual void addFrame(std::string name, const SubTexture2D& sub_texture);
		virtual const std::map<std::string, SubTexture2D>& getFrames() const;
		virtual std::list<std::string> getNames() const;
		virtual frames_iterator getFrame(std::string name);
	private:
		std::map<std::string, SubTexture2D> m_frames;
	};

	class SpriteAtlas : public Texture2D
	{
	public:
		using animations_frame = std::pair<bool, std::list<SubTexture2D>>;
		using frames_iterator = std::list<SubTexture2D>::const_iterator;
		using animations_iterator = std::map<std::string, animations_frame>::const_iterator;
		SpriteAtlas(const std::string& path,
			const unsigned int channels, const std::string& type = "texture_diffuse",
			const unsigned int textureSlot = GL_TEXTURE0,
			const GLenum filter = GL_LINEAR,
			const GLenum wrapMode = GL_CLAMP_TO_EDGE);
		~SpriteAtlas() override = default;
		virtual void addAnimation(std::string name, const animations_frame& anim);
		virtual const animations_frame& getAnimation(std::string name);
		virtual std::map<std::string, RenderEngine::SpriteAtlas::animations_frame>& getAnimations() { return m_Animations; }
	protected:
		std::map<std::string, animations_frame> m_Animations;
	};
}
