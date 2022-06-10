#pragma once
#include <functional>
#include <string>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
namespace Animation
{
	class AnimListener
	{
	public:
		virtual ~AnimListener() = default;
		virtual void AnimFinished(std::string anim) = 0;
	};

	class Anim
	{
	public:
		Anim();
		virtual ~Anim() {}
		virtual void render() {}
		virtual bool update(const float delta_time) { return false; }
		virtual void forceEndAnimation() {}
		bool unblocker;
		bool blocker;
		bool removeMe;
		float acceleration;
		std::string name;
		std::function<void()> postFunc;
		std::function<bool()> checkBlockerFunc;
		AnimListener* listener;
	};

	class AnimationsList
	{
	public:
		AnimationsList(bool update_by_tick = false);
		virtual ~AnimationsList();
		void push_back(Anim* anim, bool isUnblocker, AnimListener* listener = NULL, std::string name = "", std::function<void()> postFunc = nullptr);
		void push_back(Anim* anim, std::function<void()> postFunc = nullptr, bool isUnblocker = false);

		template<typename LambdaFunction, std::enable_if_t<std::is_same_v<decltype(std::declval<LambdaFunction>()()), void>, bool> = true>
		void push_back(Anim* anim, LambdaFunction postFunc, bool isUnblocker = false) {
			push_back(anim, std::function<void()>{postFunc}, isUnblocker);
		}

		void push_blocker(std::function<bool()> checkBlockerFunc = nullptr);
		void clear();
		void render();
		void update(const float delta_time);
		bool empty() const { return animations.empty(); };
		size_t size() const { return animations.size(); };
		std::list<Anim*>* getAnimations() { return &animations; };
		int countByName(std::string anim_name);
		void setUpdateByTick(bool isUpdateByTick);
		void setUnblockerBreak();
	private:
		std::list<Anim*> animations;
		bool unblocker;
		bool update_by_tick;
		bool unblocker_break;
	};

	enum class eAnimType: unsigned
	{
		LINEAR,
		CUBIC,
		PARABOLA,
		REVERSED_CUBIC
	};

	template<typename _T>
	class Diff : public Anim
	{
	public:
		Diff(float time, _T start, _T dest, _T* value, eAnimType interp = eAnimType::LINEAR, std::function<void()> updateFunc = nullptr) :
			updateFunc(updateFunc)
		{
			this->interp = interp;
			this->start = start;
			this->dest = dest;
			this->time = time;
			this->value = value;
			timer = 0.0f;
		}
		void forceEndAnimation() { timer = time; }
		bool update(const float delta_time) {
			bool result = false;
			timer += delta_time * acceleration;
			if (timer > time)
			{
				result = true;
				timer = time;
			}
			if (time <= 0)
				*value = dest;
			else
			{
				float f = timer / time;
				switch (interp)
				{
				case eAnimType::LINEAR:
				{
					*value = start * (1.0f - f) + dest * f;
					break;
				}
				case eAnimType::CUBIC:
				{
					f = (1 + sin((f * M_PI) - M_PI / 2)) / 2;
					*value = start * (1.0f - f) + dest * f;
					break;
				}
				case eAnimType::PARABOLA:
				{
					f = -((2 * f - 1) * (2 * f - 1)) + 1;
					*value = start + ((dest - start) * f);
					break;
				}
				case eAnimType::REVERSED_CUBIC:
				{
					f = 0.5 * (1 - (1 - f) * (1 - f) * (1 - f)) + 0.5 * f * f * f;
					*value = start + ((dest - start) * f);
					break;
				}
				}
			}
			if (updateFunc)
				updateFunc();
			return result;
		}
		float timer;
		float time;
		eAnimType interp;
		_T start;
		_T dest;
		_T* value;
		std::function<void()> updateFunc;
	};

	template<typename _T>
	class Shift : public Anim
	{
	public:
		Shift(float time, _T diff, _T* value)
		{
			this->time = time;
			this->diff = diff;
			this->value = value;
			accum = 0.0f;
		}
		bool update(const float delta_time) override
		{
			bool result = false;
			float f = (delta_time * acceleration) / time;
			if (accum + f > 1.0f)
			{
				f = 1.0f - accum;
				if (f < 0.0f)
					f = 0.0f;
				result = true;
			}
			accum += f;
			*value += diff * f;
			return result;
		}
		float time;
		_T diff;
		float accum;
		_T* value;
	};

	class Wait : public Anim
	{
	public:
		Wait(float time):
			timer(0), time(time)
		{}

		bool update(const float delta_time) override
		{
			if ((timer += (delta_time * acceleration)) > time)
				return true;
			return false;
		}

		void forceEndAnimation() { timer = time; }
		float timer;
		float time;
	};
}
