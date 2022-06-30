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
	return last_duration;
}

void UpdateSystem::Update(std::shared_ptr<RenderEngine::Sprite> sprite)
{
	if(!sprite || !sprite->getCurrentAnimation())
		return;
	sprite->getCurrentAnimation()->Update(last_duration);
	sprite->SetSubTexture(sprite->getCurrentAnimation()->getLastFrame());
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
	long nDelta;
	do {
		const long long nCurTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
		nDelta = nCurTime - nStartTime;
	} while (nDelta >= 0 && nDelta < nMillis);
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
