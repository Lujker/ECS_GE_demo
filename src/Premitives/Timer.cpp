#include "Timer.h"

Timer::Timer():
 m_begin_callback(nullptr), m_end_callback(nullptr), m_duraton(0), m_is_runinig(false)
{}

void Timer::update(const float delta_time)
{
	if(m_is_runinig)
	{
		m_duraton -= delta_time;
		if (m_duraton <= 0)
			stop();
	}
}

void Timer::start(const float duration)
{
	m_duraton = duration;
	m_is_runinig = true;
	if(m_begin_callback)
		m_begin_callback();
}

void Timer::stop()
{
	m_is_runinig = false;
	m_duraton = 0;
	if(m_end_callback)
		m_end_callback();
}

void Timer::pause()
{
	m_is_runinig = false;
}

void Timer::resume()
{
	m_is_runinig = true;
}

void Timer::setBeginCallback(const std::function<void()>& callback)
{
	m_begin_callback = callback;
}

void Timer::setEndCallBack(const std::function<void()>& callback)
{
	m_end_callback = callback;
}
