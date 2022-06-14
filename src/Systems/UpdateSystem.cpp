#include "UpdateSystem.h"

#include "Animator.h"
#include "Sprite.h"
#include "Timer.h"

std::chrono::time_point<std::chrono::steady_clock> UpdateSystem::lastTime = std::chrono::high_resolution_clock::now();
float UpdateSystem::last_duration = 0.f;

float UpdateSystem::GlobalUpdate(bool restart)
{
	if(restart)
		lastTime = std::chrono::high_resolution_clock::now();
	const auto currentTime = std::chrono::high_resolution_clock::now();
	last_duration = std::chrono::duration<float, std::milli>(currentTime - lastTime).count();
	lastTime = currentTime;
	return last_duration;
}

void UpdateSystem::Update(std::shared_ptr<RenderEngine::Sprite> sprite)
{
	if(!sprite->getCurrentAnimation())
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

UpdateSystem& UpdateSystem::Instanse()
{
    static UpdateSystem update_system;
    return update_system;
}
