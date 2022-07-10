#include "Button.h"

ButtonListener::~ButtonListener()
{
}

Button::Button(unsigned id, const PositionComponent& default_position, const CollisionComponent& default_size, const CollisionComponent& default_collision) :
	IInterfaceObject(id, default_position, default_size, default_collision), m_string(nullptr), m_bg(nullptr)
{}

Button::~Button()
{
	if (isAddedToInputManager)
		INPUTS.ListenerRemove(this);
}

void Button::addListener(ButtonListener* listener)
{
	if (listener)
		m_listeners.push_back(listener);
}

void Button::removeListener(ButtonListener* listener)
{
	const auto it = std::find(m_listeners.begin(), m_listeners.end(), listener);
	if (it != m_listeners.end())
	{
		m_listeners.erase(it);
	}
}

bool Button::isExistListener(ButtonListener* listener)
{
	const auto it = std::find(m_listeners.begin(), m_listeners.end(), listener);
	if (it != m_listeners.end())
	{
		return true;
	}
	return false;
}

void Button::setImage(std::shared_ptr<RenderEngine::Image2D> image)
{
	m_bg = image;
}

void Button::setString(std::shared_ptr<DisplayString> string)
{
	m_string = string;
}

void Button::setAutoInput(bool isAutoInput)
{
	if (isAutoInput && !isAddedToInputManager)
		INPUTS.ListenerAdd(this);
	else if (!isAutoInput && isAddedToInputManager)
		INPUTS.ListenerRemove(this);
}

void Button::setSizeString(const CollisionComponent& size_string)
{
	m_size_string = size_string;
}

void Button::setStringColor(const ColorComponent& string_color)
{
	m_string_color = string_color;
}

void Button::MouseMove(const FPoint& current_pos)
{
}

void Button::MousePress(const int& key)
{
}

void Button::MouseUnpress(const int& key)
{
}
