#pragma once
#include <chrono>
#include <map>
#include <memory>
#include "Image.h"
#include <list>

namespace RenderEngine
{
	class SpriteAtlas;

	class SpriteAnimation
	{
	public:
		using atlas = std::shared_ptr<RenderEngine::SpriteAtlas>;

		SpriteAnimation() = default;
		SpriteAnimation(atlas atlas);
		void restart();
		bool isEmpty() const;
		atlas getAtlas() const;
		bool hasAnimNames(const std::string& name) const;
		void setAnnimation(const std::string& name);
		void setAnnimation(const RenderEngine::SpriteAtlas::animations_iterator&);
		const std::pair<bool, std::list<SubTexture2D>>& getCurrentAnimation() const;
		const std::string& getCurrentAnimationName() const;
		const std::list<std::string> getAnimNames();
		SubTexture2D getLastFrame() const;
		void Update(float delta_time);
	private:
		atlas m_atlas;
		float currentFramDuration = 0;
		RenderEngine::SpriteAtlas::animations_iterator animation_iterator;
		RenderEngine::SpriteAtlas::frames_iterator frame_iterator;
		SubTexture2D lastFrame;
	};

	class Sprite : public Image2D
	{
	public:
		Sprite(std::list<SpriteAnimation> anim_list);
		bool setAnimation(const std::string& name);
		void addSpriteAnimation(const SpriteAnimation& animation);
		SpriteAnimation getSpriteAnimation(const std::string& name);
		std::shared_ptr<SpriteAnimation> getCurrentAnimation();
		std::list<std::string> getAnimationsName();

	private:
		std::shared_ptr<SpriteAnimation> m_currentAnimation;
		std::list<SpriteAnimation> m_animations;
	};
}
