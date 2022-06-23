#include "WidgetsManager.h"

WidgetManager& WidgetManager::Instanse()
{
	static WidgetManager widget_manager;
	return widget_manager;
}
