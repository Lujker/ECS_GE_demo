#include "Animator.h"

using namespace Animation;

Anim::Anim():
	postFunc(nullptr),
	checkBlockerFunc(nullptr),
	unblocker(false),
	blocker(false),
	removeMe(false),
	listener(nullptr),
	name(""),
	acceleration(1.0f)
{}

AnimationsList::AnimationsList(bool update_by_tick):
	unblocker(false),
	update_by_tick(update_by_tick),
	unblocker_break(false)
{}

AnimationsList::~AnimationsList()
{
	clear();
}

void AnimationsList::clear()
{
	unblocker = false;
	for (auto anim : animations)
		delete anim;
	animations.clear();
}

void AnimationsList::push_back(Anim* anim, bool isUnblocker, AnimListener* listener, std::string name, std::function<void()> postFunc)
{
	anim->unblocker = isUnblocker;
	anim->listener = listener;
	anim->name = name;
	anim->postFunc = postFunc;
	animations.push_back(anim);
}

void AnimationsList::push_back(Anim* anim, std::function<void()> postFunc, bool isUnblocker)
{
	push_back(anim, isUnblocker, nullptr, "", postFunc);
}

void AnimationsList::push_blocker(std::function<bool()> checkBlockerFunc)
{
	Anim* anim = new Anim();
	anim->checkBlockerFunc = checkBlockerFunc;
	anim->blocker = true;
	animations.push_back(anim);
}

void AnimationsList::render()
{
	for (auto anim : animations)
		anim->render();
}

void AnimationsList::update(const float delta_time)
{
	bool removed = false;
	for (auto it = animations.begin(); it != animations.end(); ++it)
	{
		Anim* anim = (*it);
		if (!anim->blocker)
		{
			if (anim->update(delta_time))
			{
				if (anim->unblocker)
				{
					unblocker = true;
				}
				anim->removeMe = true;
				removed = true;
			}
		}
		else
		{
			if (unblocker && (!anim->checkBlockerFunc || (anim->checkBlockerFunc && anim->checkBlockerFunc())))
			{
				unblocker = false;
				anim->removeMe = true;
				removed = true;
				if (unblocker_break)
				{
					break;
				}
			}
			else
			{
				break;
			}
		}
	}
	if (removed)
	{
		for (auto it = animations.begin(); it != animations.end();)
		{
			Anim* anim = (*it);
			if (anim->removeMe)
			{
				if (anim->listener)
				{
					anim->listener->AnimFinished(anim->name);
				}
				if (anim->postFunc)
				{
					anim->postFunc();
				}
				delete anim;
				it = animations.erase(it);
			}
			else
			{
				++it;
			}
		}
		return;
	}

	std::list<Anim*>::iterator i = animations.begin();
	while (i != animations.end())
	{
		Anim* anim = (*i);
		if (!anim->blocker)
		{
			if (anim->update(update_by_tick ? delta_time : 1.f / 60.f))
			{
				if (anim->unblocker)
					unblocker = true;
				if (anim->listener)
					anim->listener->AnimFinished(anim->name);
				if (anim->postFunc)
					anim->postFunc();
				delete anim;
				animations.erase(i++);
			}
			else
				++i;
		}
		else
		{
			if (unblocker && (!anim->checkBlockerFunc || (anim->checkBlockerFunc && anim->checkBlockerFunc())))
			{
				unblocker = false;
				delete anim;
				animations.erase(i++);
			}
			else
				break;
		}
	}
}

void AnimationsList::setUpdateByTick(bool isUpdateByTick)
{
	update_by_tick = isUpdateByTick;
}

int AnimationsList::countByName(std::string anim_name)
{
	int count = 0;
	for (const auto& anim : animations)
	{
		if (anim->name == anim_name)
			++count;
	}
	return count;
}

void AnimationsList::setUnblockerBreak() {
	unblocker_break = true;
}
