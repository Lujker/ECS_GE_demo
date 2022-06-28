#include "WidgetContainer.h"

#include "LogSystem.h"
#include "Widget.h"

WidgetContainer::WidgetContainer(const std::string& widgetName, const eWidgetPriority& priority, const std::shared_ptr<WidgetContainer>& parent):
	mParent(parent), mName(widgetName), mPriority(priority), showedAnimList(std::make_unique<Animation::AnimationsList>())
{
	INPUTS.ListenerAdd(this);
}

WidgetContainer::~WidgetContainer()
{
	INPUTS.ListenerRemove(this);
}

eWidgetPriority WidgetContainer::GetPriority() const
{
	return mPriority;
}

std::string WidgetContainer::GetName() const
{
	return mName;
}

bool WidgetContainer::IsShowed() const
{
	return mIsShowed;
}

void WidgetContainer::AddWidget(std::shared_ptr<Widget> theWidget)
{
	if (!theWidget)
		return;
	mWidgets.push_back(theWidget);
	theWidget->StartShow([&, theWidget]()
		{
			if(theWidget)
				theWidget->AddedToContainer(theWidget->mParent);
			LOG("Push widget: " + theWidget->GetName() + " to: " + mName);
		});
}

void WidgetContainer::RemoveWidget(std::shared_ptr<Widget> theWidget)
{
	if(!theWidget)
		return;
	const auto find_it = std::find(mWidgets.begin(), mWidgets.end(), theWidget);
	if (find_it != mWidgets.end())
	{
		theWidget->StartShow([&, theWidget]()
			{
				if (theWidget)
					theWidget->RemovedFromContainer(theWidget->mParent);
				mWidgets.erase(find_it);
				LOG("Remove widget: " + theWidget->GetName() + " from: " + mName);
			});
	}
}

bool WidgetContainer::HasWidget(std::shared_ptr<Widget> theWidget)
{
	if (!theWidget)
		return false;
	const auto find_it = std::find(mWidgets.begin(), mWidgets.end(), theWidget);
	if (find_it != mWidgets.end())
	{
		LOG("Find widget: " + theWidget->GetName() + " in container: " + mName);
		return true;
	}
	return false;
}

void WidgetContainer::RemoveAllWidgets(bool recursive)
{
	if(mWidgets.empty())
		return;
	if (recursive)
	{
		for(auto& widget_it : mWidgets)
		{
			widget_it->RemoveAllWidgets(true);
			RemoveWidget(widget_it);
		}
	}
	mWidgets.clear();
}

void WidgetContainer::SortWidgets()
{
	std::stable_sort(mWidgets.begin(), mWidgets.end(), [](const auto& first, const auto& second)
		{
			return (static_cast<unsigned>(first->GetPriority()) >= static_cast<unsigned>(second->GetPriority()));
		});
}

std::shared_ptr<Widget> WidgetContainer::FindWidget(const std::string& widgetName)
{
	if (widgetName.empty())
		return nullptr;
	const auto find_it = std::find_if(mWidgets.begin(), mWidgets.end(), [&](const auto& it)
	{
			return it->GetName() == widgetName;
	});
	if (find_it != mWidgets.end())
	{
		return *find_it;
	}
	return nullptr;
}

void WidgetContainer::StartShow(const std::function<void()>& callback, bool with_animation)
{
	mIsShowed = true;
	if (with_animation)
	{
		//!init anim list for appear widget
		callback();
	}
	else
	{
		callback();
	}
}

void WidgetContainer::StartClose(const std::function<void()>& callback, bool with_animation)
{
	if (with_animation)
	{
		mIsShowed = false;
		callback();
		//!init anim list for disappear widget and set isShowed == false
	}
	else
	{
		mIsShowed = false;
		callback();
	}
}

void WidgetContainer::AddedToContainer(std::shared_ptr<WidgetContainer> theWidgetManager)
{
}

void WidgetContainer::RemovedFromContainer(std::shared_ptr<WidgetContainer> theWidgetManager)
{
}

void WidgetContainer::UpdateAll(float deltaTime)
{
	if(!mIsShowed)
		return;
	Update(deltaTime);
	for(const auto& it : mWidgets)
	{
		it->UpdateAll(deltaTime);
	}
}

void WidgetContainer::DrawAll()
{
	if (!mIsShowed)
		return;
	Draw();
	for (const auto& it : mWidgets)
	{
		it->DrawAll();
	}
}
