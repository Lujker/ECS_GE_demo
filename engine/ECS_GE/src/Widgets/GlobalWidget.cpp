#include "GlobalWidget.h"
#include "Engine.h"
#include "CameraManager.h"

GlobalWidget::GlobalWidget(const std::string& widgetName, const eWidgetPriority& priority, const SharedWidgetContainer& parent):
	Widget(widgetName, priority, parent)
{
}

void GlobalWidget::OriginRectSet()
{
	const auto windRect = CAMERA->getActiveWindowRect();
	mOrigin = {
		static_cast<int>(windRect.mX), static_cast<int>(windRect.mY),
		static_cast<int>(windRect.mWidth), static_cast<int>(windRect.mHeight)};
}
