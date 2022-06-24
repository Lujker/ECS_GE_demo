#pragma once
#include "Rect.h"
#include "WidgetContainer.h"

class Widget : public WidgetContainer
{
protected:
	Rect		origin;
	bool		mVisible;
	bool		mMouseVisible;
	bool		mDisabled;
	bool		mHasFocus;
	bool		mIsDirty;
public:
	explicit		Widget(const std::string& widgetName, const eWidgetPriority& priority, const std::shared_ptr<WidgetContainer>& parent = nullptr);
	virtual			~Widget();
	virtual bool	Intersects(std::shared_ptr<Widget> theWidget);
	virtual bool	Contains(int theX, int theY);
	virtual bool	IsDirty() const;
	virtual bool	IsDisable() const;

	virtual void	OriginRectSet();
	virtual void	MarkDirty();
	virtual void	MarkDisable();
	virtual void	SetRect(Rect rect);
	virtual void	SetFocus();

	virtual std::string GetWidgetName() const;
	virtual Point	GetAbsPos();
	virtual Rect	GetRect() const;

	//////// Layout functions
	int		Left() { return origin.mX; }
	int		Top() { return origin.mY; }
	int		Right() { return origin.mX + origin.mWidth; }
	int		Bottom() { return origin.mY + origin.mHeight; }
	int		Width() { return origin.mWidth; }
	int		Height() { return origin.mHeight; }
};