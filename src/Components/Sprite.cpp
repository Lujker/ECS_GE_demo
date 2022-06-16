#include "Sprite.h"

#include <iostream>


namespace RenderEngine
{
	Sprite::Sprite(std::list<SpriteAnimation> anim_list):
		m_animations(std::move(anim_list))
	{
		if(!m_animations.empty())
		{
			setAnimation(m_animations.begin()->getCurrentAnimationName());
			init();
		}
	}

	bool Sprite::setAnimation(const std::string& name)
	{
		if (m_animations.empty())
		{
			std::cerr << "Sprite::setAnimation sprite anim list is empty" << std::endl;
			return false;
		}
		const auto it = std::find_if(m_animations.begin(), m_animations.end(), [&](const auto& iter)
			{
				return iter.hasAnimNames(name);
			});
		if (it != m_animations.end() && !it->isEmpty())
		{
			it->restart();
			m_currentAnimation = std::make_shared<SpriteAnimation>(*it);
			m_currentAnimation->setAnnimation(name);
			m_pTexture = it->getAtlas();
			m_subTexture = it->getLastFrame();
			SetSubTexture(m_subTexture);
			return true;
		}
		return false;
	}

	void Sprite::addSpriteAnimation(const SpriteAnimation& animation)
	{
		if (!animation.isEmpty())
			m_animations.push_back(animation);
		std::cerr << "Try add empty SpriteAnimation it is error" << std::endl;
	}

	SpriteAnimation Sprite::getSpriteAnimation(const std::string& name)
	{
		if (m_animations.empty())
		{
			std::cerr << "Sprite::setAnimation sprite anim list is empty" << std::endl;
			return SpriteAnimation{};
		}
		const auto it = std::find_if(m_animations.begin(), m_animations.end(), [&](const auto& iter)
			{
				return iter.hasAnimNames(name);
			});
		if (it != m_animations.end() && !it->isEmpty())		
		{
			return *it;
		}
		return SpriteAnimation{};
	}

	std::shared_ptr<SpriteAnimation> Sprite::getCurrentAnimation()
	{
		return m_currentAnimation;
	}

	std::list<std::string> Sprite::getAnimationsName()
	{
		std::list<std::string> names;
		for(const auto& it : m_animations)
		{
			for (const auto& anim : it.getAtlas()->getAnimations())
				names.push_back(anim.first);
		}
		return names;
	}

	SpriteAnimation::SpriteAnimation(atlas atlas):
		m_atlas(std::move(atlas)), currentFramDuration(0), 
		animation_iterator(m_atlas->getAnimations().begin()),
		frame_iterator(animation_iterator->second.second.begin()),
		lastFrame(*frame_iterator)
	{}

	void SpriteAnimation::restart()
	{
		currentFramDuration = 0;
		frame_iterator = animation_iterator->second.second.begin();
		lastFrame = *frame_iterator;
	}

	bool SpriteAnimation::isEmpty() const
	{
		if (!m_atlas && animation_iterator != m_atlas->getAnimations().end() && frame_iterator != animation_iterator->second.second.end())
			return true;
		return false;
	}
	SpriteAnimation::atlas SpriteAnimation::getAtlas() const
	{
		return m_atlas;
	}

	bool SpriteAnimation::hasAnimNames(const std::string& name) const
	{
		const auto& it = m_atlas->getAnimations().find(name);
		if (it != m_atlas->getAnimations().end())	return true;
		return false;
	}

	void SpriteAnimation::setAnnimation(const std::string& name)
	{
		const auto& it = m_atlas->getAnimations().find(name);
		if ( it != m_atlas->getAnimations().end())
		{
			animation_iterator = it;
			currentFramDuration = 0;
			frame_iterator = animation_iterator->second.second.begin();
			lastFrame = *frame_iterator;
			return;
		}
		std::cerr << "SpriteAnimation::setAnnimation invalide name set" << std::endl;
	}

	void SpriteAnimation::setAnnimation(const RenderEngine::SpriteAtlas::animations_iterator& iter)
	{
		if (iter != m_atlas->getAnimations().end())
		{
			animation_iterator = iter;
			currentFramDuration = 0;
			frame_iterator = animation_iterator->second.second.begin();
			lastFrame = *frame_iterator;
			return;
		}
		std::cerr << "SpriteAnimation::setAnnimation invalide iterator set" << std::endl;
	}

	const std::pair<bool, std::list<SubTexture2D>>& SpriteAnimation::getCurrentAnimation() const
	{
		return animation_iterator->second;
	}

	const std::string& SpriteAnimation::getCurrentAnimationName() const
	{
		return animation_iterator->first;
	}

	const std::list<std::string> SpriteAnimation::getAnimNames()
	{
		std::list<std::string> names;
		for(const auto& it : m_atlas->getAnimations())
		{
			names.push_back(it.first);
		}
		return names;
	}

	SubTexture2D SpriteAnimation::getLastFrame() const
	{
		return lastFrame;
	}
	void SpriteAnimation::Update(float delta_time)
	{
		currentFramDuration += delta_time;
		if (currentFramDuration >= lastFrame.duration)
		{
			currentFramDuration -= lastFrame.duration;
			const auto &nextIter = (++frame_iterator);
			if (nextIter == animation_iterator->second.second.end())
			{
				if (animation_iterator->second.first)
					frame_iterator = animation_iterator->second.second.begin();
				else
					--frame_iterator;
			}
			if(lastFrame != *frame_iterator)
				lastFrame = *frame_iterator;
		}
	}
}
