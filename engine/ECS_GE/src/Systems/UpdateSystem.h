#pragma once
#include <chrono>
#include <memory>
#include "ISystem.h"


namespace RenderEngine
{
	class Sprite;
}

class Timer;
class FPSCounter
{
public:
	FPSCounter();
	void Update(float delta_time);
	float GetTimer() const;
	unsigned GetFPS() const;
private:
	float timer = 0.f;
	unsigned counter = 0u;
	unsigned lastFPS = 0u;
};

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
	[[nodiscard]] unsigned GetFPS() const;

	UpdateSystem(const UpdateSystem&) = delete;
	UpdateSystem(UpdateSystem&&) = delete;
	UpdateSystem& operator=(const UpdateSystem&) = delete;
	UpdateSystem& operator=(UpdateSystem&&) = delete;
private:
	std::chrono::time_point<std::chrono::steady_clock> lastTime;
	float last_duration = 0.f;
	bool pause = false;
	FPSCounter fps;

	UpdateSystem();
	~UpdateSystem() override = default;
};
