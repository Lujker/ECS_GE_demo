#pragma once
#include "Collision.h"
#include "Collor.h"
#include "DisplayString.h"
#include "GameObject.h"
#include "InputManager.h"
#include "Move.h"
#include "Position.h"

namespace RenderEngine
{
	class Image2D;
}

struct ButtonListener
{
	virtual ~ButtonListener() = 0 ;
	virtual void ButtonPressed(unsigned id) { }
	virtual void ButtonUnpressed(unsigned id) { }
	virtual void MouseMoveOnButton(unsigned id) { }
};

class Button : public IInterfaceObject
{
public:
					Button(unsigned id, const PositionComponent& default_position,
							const CollisionComponent& default_size, 
							const CollisionComponent& default_collision);
	virtual			~Button() override;
	virtual void	addListener(ButtonListener*);
	virtual void	removeListener(ButtonListener*);
	virtual bool	isExistListener(ButtonListener*);

	virtual void	setImage(std::shared_ptr<RenderEngine::Image2D>);
	virtual void	setString(std::shared_ptr<DisplayString>);
	virtual void	setAutoInput(bool);
	virtual void	setSizeString(const CollisionComponent& size_string);
	virtual void	setStringColor(const ColorComponent& string_color);

	std::shared_ptr<RenderEngine::Image2D>	getImage() const { return m_bg; }
	std::shared_ptr<DisplayString>			getString() const { return m_string; }
	CollisionComponent						getStringSize() const { return m_size_string; }
	bool									isAutoInput() const { return isAddedToInputManager; }

	virtual void	MouseMove(const FPoint& current_pos) override;
	virtual void	MousePress(const int& key) override;
	virtual void	MouseUnpress(const int& key) override;
protected:
	std::shared_ptr<DisplayString>			m_string;
	std::shared_ptr<RenderEngine::Image2D>	m_bg;
	CollisionComponent						m_size_string;
	ColorComponent							m_string_color;
	std::list<ButtonListener*>				m_listeners;
	bool									isAddedToInputManager = false;
};
