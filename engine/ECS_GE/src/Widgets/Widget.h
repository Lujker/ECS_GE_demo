#pragma once
#include "Rect.h"
#include "WidgetContainer.h"

class Widget : public WidgetContainer
{
	Rect	rect;
	bool	mVisible;
	bool	mMouseVisible;
	bool	mDisabled;
	bool	mHasFocus;
	bool	mIsDown;
	bool	mIsOver;
	bool	mIsDirty;
public:
					Widget();
	virtual			~Widget();
	virtual void	SetRect(Rect rect);
	virtual bool	Contains(int theX, int theY);
	virtual bool	Intersects(std::shared_ptr<Widget> theWidget);
	virtual void	SetFocus(std::shared_ptr<Widget> theWidget);
	virtual bool	IsBelow(std::shared_ptr<Widget> theWidget1, std::shared_ptr<Widget> theWidget2);
	virtual void	BringToFront(std::shared_ptr<Widget> theWidget);
	virtual void	BringToBack(std::shared_ptr<Widget> theWidget);
	virtual void	PutBehind(std::shared_ptr<Widget> theWidget, std::shared_ptr<Widget> theRefWidget);
	virtual void	PutInfront(std::shared_ptr<Widget> theWidget, std::shared_ptr<Widget> theRefWidget);
	virtual Point	GetAbsPos(); // relative to top level
	virtual Rect	GetRect() const;
	virtual void	MarkDirty();

	//////// Layout functions
	int		Left() { return rect.mX; }
	int		Top() { return rect.mY; }
	int		Right() { return rect.mX + rect.mWidth; }
	int		Bottom() { return rect.mY + rect.mHeight; }
	int		Width() { return rect.mWidth; }
	int		Height() { return rect.mHeight; }
};