#pragma once
#include <chrono>

#include "ISystem.h"


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
	static float GlobalUpdate();
	static void Update(Animation::AnimationsList& anim_list);
	static void Update(Timer& timer);
private:
	static std::chrono::time_point<std::chrono::steady_clock> lastTime;
	static float last_duration;
	UpdateSystem() = default;
	~UpdateSystem() = default;
	UpdateSystem(const UpdateSystem&) = delete;
	UpdateSystem(UpdateSystem&&) = delete;
	UpdateSystem& operator=(const UpdateSystem&) = delete;
	UpdateSystem& operator=(UpdateSystem&&) = delete;
};
