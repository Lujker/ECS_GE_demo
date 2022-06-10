#pragma once

#include "functional"

class Timer
{
public:
	Timer();
	void update(const float delta_time);
	void start(const float duration);
	void stop();
	void pause();
	void resume();

	void setBeginCallback(const std::function<void()>& callback);
	void setEndCallBack(const std::function<void()>& callback);
private:
	std::function<void()> m_begin_callback;
	std::function<void()> m_end_callback;
	float m_duraton;
	bool m_is_runinig;
};