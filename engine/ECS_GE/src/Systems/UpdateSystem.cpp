#include "UpdateSystem.h"

#include "Animator.h"
#include "CameraManager.h"
#include "Sprite.h"
#include "Timer.h"

float UpdateSystem::GlobalUpdate(bool restart)
{
	if(restart)
		lastTime = std::chrono::high_resolution_clock::now();
	const auto currentTime = std::chrono::high_resolution_clock::now();
	if (!pause && CAMERA.windowOnFocus()) 
	{
		last_duration = std::chrono::duration<float, std::milli>(currentTime - lastTime).count();
	}
	else
		last_duration = 0;
	lastTime = currentTime;
	fps.Update(last_duration);
	return last_duration;
}

void UpdateSystem::Update(std::shared_ptr<RenderEngine::Sprite> sprite)
{
	if(!sprite || !sprite->getCurrentAnimation())
		return;
	if (sprite->getIsCurrentAnimEnd())
	{
		sprite->emitEndAnimation(sprite->getCurrentAnimName());
		if (const auto animName = sprite->nextAnimInQueue(); !animName.empty() && sprite->hasAnimation(animName))
		{
			sprite->setAnimation(animName);
		}
	}
	sprite->getCurrentAnimation()->Update(last_duration);
	sprite->SetSubTexture(sprite->getCurrentAnimation()->getLastFrame());
	if (sprite->isMirrored())
	{
		const GLfloat vertex_coords[] =
		{
			//! X and Y
			sprite->isHorizontalMirror() ? 1.f : 0.f, sprite->isVerticalMirror() ? 1.f : 0.f,
			sprite->isHorizontalMirror() ? 1.f : 0.f, sprite->isVerticalMirror() ? 0.f : 1.f,
			sprite->isHorizontalMirror() ? 0.f : 1.f, sprite->isVerticalMirror() ? 0.f : 1.f,
			sprite->isHorizontalMirror() ? 0.f : 1.f, sprite->isVerticalMirror() ? 1.f : 0.f,
		};
		sprite->updateVertex(vertex_coords, sizeof(vertex_coords));
	}
}

void UpdateSystem::Update(Animation::AnimationsList& anim_list)
{
	anim_list.update(last_duration);
}

void UpdateSystem::Update(Timer& timer)
{
	timer.update(last_duration);
}

void UpdateSystem::Pause()
{
	pause = true;
}

void UpdateSystem::Resume()
{
	pause = false;
}

void UpdateSystem::Pause(long nMillis)
{
	const long long nStartTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
	long nDelta = 0;
	do {
		const long long nCurTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
		nDelta = nCurTime - nStartTime;
	} while (nDelta >= 0 && nDelta < nMillis);
}

unsigned UpdateSystem::GetFPS() const
{
	return fps.GetFPS();
}

UpdateSystem::UpdateSystem():
	lastTime(std::chrono::high_resolution_clock::now())
{
}

UpdateSystem& UpdateSystem::Instanse()
{
    static UpdateSystem update_system;
    return update_system;
}

FPSCounter::FPSCounter() : timer(0.f), counter(0), lastFPS(0)
{
}

void FPSCounter::Update(float delta_time)
{
	if (delta_time > 0.f)
	{
		timer += delta_time;
		++counter;
		if (timer >= 1000)
		{
			lastFPS = counter;
			counter = 0u;
			timer = 0.f;
		}
	}
}

float FPSCounter::GetTimer() const
{
	return timer;
}

unsigned FPSCounter::GetFPS() const 
{
	return lastFPS;
}
