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
	static float GlobalUpdate(bool restart = false);
	static void Update(std::shared_ptr<RenderEngine::Sprite> sprite);
	static void Update(Animation::AnimationsList& anim_list);
	static void Update(Timer& timer);
	static void Pause();
	static void Resume();
private:
	static std::chrono::time_point<std::chrono::steady_clock> lastTime;
	static float last_duration;
	static bool pause;
	UpdateSystem() = default;
	~UpdateSystem() = default;
	UpdateSystem(const UpdateSystem&) = delete;
	UpdateSystem(UpdateSystem&&) = delete;
	UpdateSystem& operator=(const UpdateSystem&) = delete;
	UpdateSystem& operator=(UpdateSystem&&) = delete;
};
