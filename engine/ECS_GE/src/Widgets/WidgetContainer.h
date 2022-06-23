#pragma once
#include <list>
#include <string>
#include <memory>

#include "InputManager.h"
#include "Point.h"

class Widget;
class WidgetManager;

typedef std::list<std::shared_ptr<Widget>> WidgetList;

class WidgetContainer : public InputListener
{
	WidgetList				mWidgets;
	std::string				mName;
	int						mPriority;
	WidgetList::iterator	mUpdateIterator;
public:
	WidgetContainer();
	virtual ~WidgetContainer();

	virtual void	AddWidget(std::shared_ptr<Widget> theWidget);
	virtual void	RemoveWidget(std::shared_ptr<Widget> theWidget);
	virtual bool	HasWidget(std::shared_ptr<Widget> theWidget);
	virtual void	DisableWidget(std::shared_ptr<Widget> theWidget);
	virtual void	RemoveAllWidgets(bool doDelete = false, bool recursive = false);

	//!Callback's
	virtual void	StartShow();
	virtual void	StartClose();
	virtual void	AddedToContainer(std::shared_ptr<WidgetContainer> theWidgetManager);
	virtual void	RemovedFromContainer(std::shared_ptr<WidgetContainer> theWidgetManager);

	virtual void	Update(float deltaTime);
	virtual void	UpdateAll(float deltaTime);
	virtual void	Draw();
	virtual void	DrawAll();

	virtual void KeyPress(const int& key) override;
	virtual void KeyUnpress(const int& key) override;
	virtual void MousePress(const int& key) override;
	virtual void MouseUnpress(const int& key) override;
	virtual void MouseMove(const FPoint& current_pos) override;
};