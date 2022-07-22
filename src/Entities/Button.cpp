#include "Button.h"

#include "CollisionSystem.h"
#include "RenderSystem.h"

ButtonListener::~ButtonListener()
{
}

Button::Button(unsigned id, const PositionComponent& default_position, const CollisionComponent& default_size, const CollisionComponent& default_collision) :
	IInterfaceObject(id, default_position, default_size, default_collision), m_string(nullptr), m_bg(nullptr), m_pressed_anim(std::make_shared<Animation::AnimationsList>())
{
	isDrawCollision = true;
	m_size_string = { m_size.getWidth() / 2 * GetSize().getScale(), m_size.getHeight() / 2 * GetSize().getScale(),
		m_size.getWidth() / 4 * GetSize().getScale(), m_size.getHeight() / 2 * GetSize().getScale(), false , 0.8f };
}

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
		m_listeners.erase(std::remove(m_listeners.begin(), m_listeners.end(), *it), m_listeners.end());
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

void Button::Draw()
{
	IInterfaceObject::Draw();
	RENDER.Render(m_bg, m_position, m_size);
	RENDER.Render(m_string, m_position, m_size_string, m_string_color);
}

void Button::Update(const float& delta_time)
{
	IInterfaceObject::Update(delta_time);
	if (m_pressed_anim)
		m_pressed_anim->update(delta_time);
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
	{
		isAddedToInputManager = true;
		INPUTS.ListenerAdd(this);
	}
	else if (!isAutoInput && isAddedToInputManager)
	{
		isAddedToInputManager = false;
		INPUTS.ListenerRemove(this);
	}
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
	if (m_collision.hasCollision() && COLLISION.include2D(m_collision, m_collision.getScale(), m_position, current_pos))
	{
		isCursorOnButton = true;
		for (const auto& it : m_listeners)
			it->MouseMoveOnButton(m_id);
	}
	else
	{
		isCursorOnButton = false;
	}
}

void Button::MousePress(const int& key)
{
	if (m_collision.hasCollision() && isCursorOnButton && !m_listeners.empty() && !isPressed)
	{
		const auto next_x = m_position.getPosition().mX + ((m_size.getWidth() * GetSize().getScale()) - m_size.getWidth() * (GetSize().getScale() - 0.2f)) / 2.;
		const auto next_y = m_position.getPosition().mY + ((m_size.getHeight() * GetSize().getScale()) - m_size.getHeight() * (GetSize().getScale() - 0.2f)) / 2.;
		isPressed = true;
		m_pressed_anim->push_back(new Animation::Wait(0), [&]() {});
		m_pressed_anim->push_back(new Animation::Diff(100.f, m_position.getPosition().mX, next_x, &(m_position.getPositionPtr()->mX),Animation::eAnimType::LINEAR,[&]()
		{
				m_size_string = { m_size.getWidth() / 2 * GetSize().getScale(), m_size.getHeight() / 2 * GetSize().getScale(), m_size.getWidth() / 4 * GetSize().getScale(), m_size.getHeight() / 2 * GetSize().getScale(), false , 0.8f };
		}));
		m_pressed_anim->push_back(new Animation::Diff(100.f, m_position.getPosition().mY, next_y, &(m_position.getPositionPtr()->mY)));
		m_pressed_anim->push_back(new Animation::Diff(100.f, m_size.getScale(), m_size.getScale() - 0.2f, m_size.getScalePtr()));
		m_pressed_anim->push_back(new Animation::Diff(100.f, m_size_string.getScale(), m_size_string.getScale() - 0.2f, m_size_string.getScalePtr()));
		m_pressed_anim->push_back(new Animation::Diff(100.f, m_collision.getScale(), m_collision.getScale() - 0.2f, m_collision.getScalePtr()),[&, key]()
		{
				if (!m_listeners.empty())
					for (const auto& it : m_listeners)
						it->ButtonPressed(m_id, key);
			
		}, true);
		m_pressed_anim->push_blocker();
	}
}

void Button::MouseUnpress(const int& key)
{
	if (m_collision.hasCollision() && isPressed && !isUnpressed)
	{
		m_pressed_anim->push_back(new Animation::Wait(0), [&, key]()
		{
			auto next_x = m_position.getPosition().mX - (m_size.getWidth() * (GetSize().getScale() + 0.2f) - (m_size.getWidth() * GetSize().getScale())) / 2.;
			auto next_y = m_position.getPosition().mY - (m_size.getHeight() * (GetSize().getScale() + 0.2f) - (m_size.getHeight() * GetSize().getScale())) / 2.;
			isUnpressed = true;
			m_pressed_anim->push_back(new Animation::Diff(100.f, m_position.getPosition().mX, next_x, &(m_position.getPositionPtr()->mX), Animation::eAnimType::LINEAR, [&]()
				{
					m_size_string = { m_size.getWidth() / 2 * GetSize().getScale(), m_size.getHeight() / 2 * GetSize().getScale(), m_size.getWidth() / 4 * GetSize().getScale(), m_size.getHeight() / 2 * GetSize().getScale(), false , 0.8f };
				}));
			m_pressed_anim->push_back(new Animation::Diff(100.f, m_position.getPosition().mY, next_y, &(m_position.getPositionPtr()->mY)));
			m_pressed_anim->push_back(new Animation::Diff(100.f, m_collision.getScale(), m_collision.getScale() + 0.2f, m_collision.getScalePtr()));
			m_pressed_anim->push_back(new Animation::Diff(100.f, m_size_string.getScale(), m_size_string.getScale() + 0.2f, m_size_string.getScalePtr()));
			m_pressed_anim->push_back(new Animation::Diff(100.f, m_size.getScale(), m_size.getScale() + 0.2f, m_size.getScalePtr()), [&, key]()
				{
					if (!m_listeners.empty())
						for (const auto& it : m_listeners)
							it->ButtonUnpressed(m_id, key);

					isPressed = false;
					isUnpressed = false;
				});
		});
	}
}
