#include "UpdateSystem.h"

#include "Animator.h"
#include "CameraManager.h"
#include "Sprite.h"
#include "Timer.h"

std::chrono::time_point<std::chrono::steady_clock> UpdateSystem::lastTime = std::chrono::high_resolution_clock::now();
float UpdateSystem::last_duration = 0.f;
bool UpdateSystem::pause = false;

float UpdateSystem::GlobalUpdate(bool restart)
{
	if(restart)
		lastTime = std::chrono::high_resolution_clock::now();
	const auto currentTime = std::chrono::high_resolution_clock::now();
	if (!pause && CAMERA.windowOnFocus())
		last_duration = std::chrono::duration<float, std::milli>(currentTime - lastTime).count();
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

UpdateSystem& UpdateSystem::Instanse()
{
    static UpdateSystem update_system;
    return update_system;
}
