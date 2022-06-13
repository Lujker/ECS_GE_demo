#pragma once
#include <map>
#include <memory>


namespace RenderEngine
{
	class TextureAtlas;

	class SpriteAnimation
	{
	public:
		using atlas = std::shared_ptr<RenderEngine::TextureAtlas>;
		SpriteAnimation();
		SpriteAnimation(atlas atlas);
		bool isEmpty() const;

	private:
		atlas m_atlas;
	};

	class Sprite
	{
	};
}
