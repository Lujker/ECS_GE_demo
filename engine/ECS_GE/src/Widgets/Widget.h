#pragma once
#include "Rect.h"
#include "RenderSystem.h"
#include "WidgetContainer.h"

class Widget : public WidgetContainer
{
	friend class WidgetManager;
protected:
	Rect		mOrigin;
	Transform2D mTransform;
	bool		mMouseVisible;
	bool		mDisabled;
	bool		mHasFocus;
	bool		mIsDirty;
public:
	explicit		Widget(const std::string& widgetName, const eWidgetPriority& priority, const std::shared_ptr<WidgetContainer>& parent = nullptr);
					~Widget() override = default;
	virtual bool	Intersects(std::shared_ptr<Widget> theWidget);
	virtual bool	Contains(int theX, int theY);
	virtual bool	IsDirty() const;
	virtual bool	IsDisable() const;

	virtual void	OriginRectSet() = 0;
	virtual void	MarkDirty();
	virtual void	MarkDisable();
	virtual void	SetRect(Rect rect);
	virtual void	SetFocus();

	virtual std::string GetWidgetName() const;
	virtual Point	GetAbsPos();
	virtual Rect	GetRect() const;

	//!Callback's
	void	StartShow(const std::function<void()>& callback, bool with_animation = false) override;
	void	StartClose(const std::function<void()>& callback, bool with_animation = false) override;
	void	AddedToContainer(std::shared_ptr<WidgetContainer> theWidgetManager) override;
	void	RemovedFromContainer(std::shared_ptr<WidgetContainer> theWidgetManager) override;

	//////// Layout functions
	int		Left() { return mOrigin.mX; }
	int		Top() { return mOrigin.mY; }
	int		Right() { return mOrigin.mX + mOrigin.mWidth; }
	int		Bottom() { return mOrigin.mY + mOrigin.mHeight; }
	int		Width() { return mOrigin.mWidth; }
	int		Height() { return mOrigin.mHeight; }
};

typedef std::shared_ptr<Widget> SharedWidget;