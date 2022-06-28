#pragma once
#include "Widget.h"

class GlobalWidget : public Widget
{
public:
	explicit GlobalWidget(const std::string& widgetName, const eWidgetPriority& priority, const std::shared_ptr<WidgetContainer>& parent = nullptr);
	virtual ~GlobalWidget() override = default;
	virtual void	OriginRectSet() override;

	//!Callback's
	//virtual void	StartShow(const std::function<void()>& callback, bool with_animation = false) override;
	//virtual void	StartClose(const std::function<void()>& callback, bool with_animation = false) override;
	//virtual void	AddedToContainer(std::shared_ptr<WidgetContainer> theWidgetManager) override;
	//virtual void	RemovedFromContainer(std::shared_ptr<WidgetContainer> theWidgetManager) override;
};
