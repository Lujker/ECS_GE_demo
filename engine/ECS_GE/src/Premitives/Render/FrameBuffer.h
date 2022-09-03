#pragma once

namespace RenderEngine 
{
	class FrameBuffer
	{
	public:
		FrameBuffer(const unsigned int& windowWidth, const unsigned int& windowHeight);
		~FrameBuffer() = default;
		void bind();
		void unbind();
		void draw();
		unsigned int getTextureId();

	private:
		void init(const unsigned int& windowWidth, const unsigned int& windowHeight);
		unsigned int framebuffer;
		unsigned int textureCollorBuffer;
		unsigned int renderBuffer;
		unsigned int quadVAO, quadVBO;
	};
}