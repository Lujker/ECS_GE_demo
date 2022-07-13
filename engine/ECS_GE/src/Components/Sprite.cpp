#include "Sprite.h"

#include <iostream>

#include "LogSystem.h"


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
			m_currentAnimName = name;
			m_currentAnimation = std::make_shared<SpriteAnimation>(*it);
			m_currentAnimation->setAnnimation(name);
			m_pTexture = it->getAtlas();
			//SetSubTexture(it->getLastFrame());
			emitStartAnimation(name);
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

	FPoint Sprite::getCurrentAnimSize()
	{
		if (m_currentAnimation && !m_currentAnimName.empty())
			return m_currentAnimation->getSize(m_currentAnimName);
		return FPoint{};
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

	bool Sprite::hasAnimation(const std::string& name)
	{
		auto animNameList = getAnimationsName();
		const auto& it = std::find(animNameList.begin(), animNameList.end(), name);
		if (it != animNameList.end())
			return true;
		return false;
	}

	std::string Sprite::getCurrentAnimName() const
	{
		return m_currentAnimName;
	}

	bool Sprite::getIsCurrentAnimEnd() const
	{
		if (m_currentAnimation && !m_currentAnimName.empty())
			return m_currentAnimation->IsEnd();
		return false;
	}

	void Sprite::addListener(SpriteAnimListener* listener)
	{
		const auto it = std::find(m_listeners.begin(), m_listeners.end(), listener);
		if (it == m_listeners.end())
		{
			m_listeners.push_back(listener);
		}
	}

	void Sprite::removeListener(SpriteAnimListener* listener)
	{
		const auto it = std::find(m_listeners.begin(), m_listeners.end(), listener);
		if (it != m_listeners.end())
		{
			m_listeners.erase(std::remove(m_listeners.begin(), m_listeners.end(), *it), m_listeners.end());
		}
	}

	void Sprite::emitStartAnimation(const std::string& name) const
	{
		for (const auto& lst : m_listeners)
		{
			lst->IsAnimationStart(name);
		}
	}

	void Sprite::emitEndAnimation(const std::string& name) const
	{
		for (const auto& lst : m_listeners)
		{
			lst->IsAnimationEnd(name);
		}
	}

	std::string Sprite::nextAnimInQueue()
	{
		if (!m_AnimQueue.empty()) 
		{
			m_AnimQueue.pop();
			return m_AnimQueue.front();
		}
		return "";
	}

	void Sprite::setAnimQueue(const std::queue<std::string>& m_animtions)
	{
		m_AnimQueue = m_animtions;
		if (!m_AnimQueue.empty())
			setAnimation(m_AnimQueue.front());
	}

	void Sprite::mirror(bool vertical, bool horizontal)
	{
		is_mirrored = true;
		m_isHorizontalMirror = horizontal;
		m_isVerticalMirror = vertical;
	}

	SpriteAnimListener::~SpriteAnimListener()
	{
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

	FPoint SpriteAnimation::getSize(const std::string& name)
	{
		const auto sub_text = m_atlas->getAnimation(name).second.begin();
		const auto width = sub_text->rightTopUV.x * m_atlas->getWidth() - sub_text->leftBottomUV.x * m_atlas->getWidth();
		const auto height = sub_text->rightTopUV.y * m_atlas->getHeight() - sub_text->leftBottomUV.y * m_atlas->getHeight();
		return { width, height };
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
				{
					m_isAnimEnd = true;
					--frame_iterator;
				}
			}
			if(lastFrame != *frame_iterator)
				lastFrame = *frame_iterator;
		}
	}
}
