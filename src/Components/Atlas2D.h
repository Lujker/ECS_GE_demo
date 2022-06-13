#pragma once
#include "IComponent.h"
#include "texture2D.h"

namespace RenderEngine
{
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


		SubTexture2D() :
			leftBottomUV(0.f), rightTopUV(1.f), duration(0)
		{}

		bool operator==(SubTexture2D right) { return this->rightTopUV == right.rightTopUV && this->leftBottomUV == right.leftBottomUV; }
		bool operator!=(SubTexture2D right) { return !((*this) == right); }
	};

	class TextureAtlas : public Texture2D
	{
	public:
		TextureAtlas(const GLuint width, GLuint heigh,
			const unsigned char* data,
			const unsigned int channels,
			const GLenum filter = GL_LINEAR,
			const GLenum wrapMode = GL_CLAMP_TO_EDGE,
			const unsigned int textureSlot = GL_TEXTURE0);
		~TextureAtlas() override = default;
		virtual const SubTexture2D& addSubTexture(std::string name, const glm::vec2& leftBottomUV, const glm::vec2& rightTopUV);
		virtual void addSubTexture(std::string name, const SubTexture2D&);
		virtual const SubTexture2D& getSubTexture(std::string name);
		virtual const std::map<std::string, SubTexture2D>& getSubTextures() { return m_subTextures; }
	protected:
		std::map<std::string, SubTexture2D> m_subTextures;
		bool isLoop = false;
	};
}
