#pragma once
#include <chrono>
#include <memory>
#include "ISystem.h"


namespace RenderEngine
{
	class Sprite;
}

class Timer;

namespace Animation
{
	class AnimationsList;
}

#define UPDATE UpdateSystem::Instanse()
class UpdateSystem : public ISystem
{
public:
	static UpdateSystem& Instanse();
	float GlobalUpdate(bool restart = false);
	void Update(std::shared_ptr<RenderEngine::Sprite> sprite);
	void Update(Animation::AnimationsList& anim_list);
	void Update(Timer& timer);
	void Pause();
	void Resume();
	void Pause(long nMillis);
private:
	std::chrono::time_point<std::chrono::steady_clock> lastTime;
	float last_duration = 0.f;
	bool pause = false;
	UpdateSystem();
	~UpdateSystem() = default;
	UpdateSystem(const UpdateSystem&) = delete;
	UpdateSystem(UpdateSystem&&) = delete;
	UpdateSystem& operator=(const UpdateSystem&) = delete;
	UpdateSystem& operator=(UpdateSystem&&) = delete;
};
