#include "Widget.h"

Widget::Widget(const std::string& widgetName, const eWidgetPriority& priority, const std::shared_ptr<WidgetContainer>& parent):
	WidgetContainer(widgetName, priority, parent), mMouseVisible(true), mDisabled(false), mHasFocus(false), mIsDirty(false)
{}

bool Widget::Intersects(std::shared_ptr<Widget> theWidget)
{
	return mOrigin.Intersects(theWidget->GetRect());
}

bool Widget::Contains(int theX, int theY)
{
	return mOrigin.Contains(theX, theY);
}

bool Widget::IsDirty() const
{
	return mIsDirty;
}

bool Widget::IsDisable() const
{
	return mDisabled;
}

void Widget::OriginRectSet()
{}

void Widget::MarkDirty()
{
	mIsDirty = true;
}

void Widget::MarkDisable()
{
	mDisabled = true;
}

void Widget::SetRect(Rect rect)
{
	mOrigin = rect;
}

void Widget::SetFocus()
{
	mHasFocus = true;
}

std::string Widget::GetWidgetName() const
{
	return WidgetContainer::mName;
}

Point Widget::GetAbsPos()
{
	return { mOrigin.mX, mOrigin.mY };
}

Rect Widget::GetRect() const
{
	return mOrigin;
}

void Widget::StartShow(const std::function<void()>& callback, bool with_animation)
{
	WidgetContainer::StartShow(callback, with_animation);
}

void Widget::StartClose(const std::function<void()>& callback, bool with_animation)
{
	WidgetContainer::StartClose(callback, with_animation);
}

void Widget::AddedToContainer(std::shared_ptr<WidgetContainer> theWidgetManager)
{
	mDisabled = false;
	mHasFocus = true;
	mIsDirty = false;
}

void Widget::RemovedFromContainer(std::shared_ptr<WidgetContainer> theWidgetManager)
{
	mDisabled = false;
	mHasFocus = true;
	mIsDirty = false;
}
