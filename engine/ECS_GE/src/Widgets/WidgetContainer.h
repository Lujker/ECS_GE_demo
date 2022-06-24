#pragma once
#include <functional>
#include <list>
#include <string>
#include <memory>
#include <queue>

#include "InputManager.h"
#include "Point.h"

namespace Animation
{
	class AnimationsList;
}

class Widget;
class WidgetManager;

typedef std::list<std::shared_ptr<Widget>> WidgetList;

enum class eWidgetPriority : unsigned
{
	TUTORIAL = 1,
	DIALOG,
	MENU,
	DEFAULT
};

class WidgetContainer : public InputListener
{
protected:
	WidgetList				mWidgets;
	std::shared_ptr<WidgetContainer>
							mParent;
	std::string				mName;
	eWidgetPriority			mPriority;
	WidgetList::iterator	mUpdateIterator;
	bool					mIsShowed = false;
	std::unique_ptr<Animation::AnimationsList>
							showedAnimList;
public:
	explicit WidgetContainer(const std::string& widgetName, const eWidgetPriority& priority, const std::shared_ptr<WidgetContainer>& parent = nullptr);
	virtual ~WidgetContainer() override = default;

	eWidgetPriority GetPriority() const;
	std::string		GetName() const;
	bool			IsShowed() const;

	void	AddWidget(std::shared_ptr<Widget> theWidget);
	void	RemoveWidget(std::shared_ptr<Widget> theWidget);
	bool	HasWidget(std::shared_ptr<Widget> theWidget);
	void	RemoveAllWidgets(bool recursive = false);
	void	SortWidgets();
	virtual	std::shared_ptr<Widget>
					FindWidget(const std::string& widgetName);

	//!Callback's
	virtual void	StartShow(const std::function<void()>& callback, bool with_animation = false);
	virtual void	StartClose(const std::function<void()>& callback, bool with_animation = false);
	virtual void	AddedToContainer(std::shared_ptr<WidgetContainer> theWidgetManager);
	virtual void	RemovedFromContainer(std::shared_ptr<WidgetContainer> theWidgetManager);

	virtual void	Update(float deltaTime) {} //for this widget callback
	virtual void	UpdateAll(float deltaTime); //for contains widgets
	virtual void	Draw() {} //for this widget callback
	virtual void	DrawAll(); //for contains widgets
};