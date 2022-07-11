#include "MainMenuWindow.h"

#include "CameraManager.h"
#include "CollisionSystem.h"
#include "LogSystem.h"
#include "RenderSystem.h"
#include "ResourceManager.h"
#include "SandboxWindow.h"
#include "WidgetsManager.h"

std::shared_ptr<MainMenuWindow> MainMenuWindow::Create()
{
	return std::make_shared<MainMenuWindow>();
}

MainMenuWindow::MainMenuWindow():
	GlobalWidget("MainMenu", eWidgetPriority::MENU, nullptr)
{
}

void MainMenuWindow::OriginRectSet()
{
	GlobalWidget::OriginRectSet();
	CAMERA.SetWorldRect(CAMERA.getActiveWindowRect());
}

void MainMenuWindow::AddedToContainer(std::shared_ptr<WidgetContainer> theWidgetContainer)
{
	CollisionComponent size{200,200, 0, 0};
	auto button = std::make_shared<Button>(0, PositionComponent{ mOrigin.mX + mOrigin.mWidth / 2 - 200, mOrigin.mY + mOrigin.mHeight / 2 }, size, size);
	button->setImage(RES.getSharedImage("res/images/test_button.png"));
	button->setString(std::make_shared<DisplayString>("button"));
	button->setAutoInput(true);
	button->addListener(this);
	m_objects.push_back(button);
	for (const auto& it : m_objects)
	{
		it->Init();
	}
}

void MainMenuWindow::RemovedFromContainer(std::shared_ptr<WidgetContainer> theWidgetContainer)
{
}

void MainMenuWindow::Draw()
{
	for (const auto& it : m_objects)
	{
		it->Draw();
	}
}

void MainMenuWindow::Update(float deltaTime)
{
	for (const auto& it : m_objects)
	{
		it->Update(deltaTime);
	}
}

void MainMenuWindow::ButtonPressed(unsigned id, const int& key)
{
}

void MainMenuWindow::ButtonUnpressed(unsigned id, const int& key)
{
	if (id == 0 && key == GLFW_MOUSE_BUTTON_LEFT)
	{
		WIDGET.SetNextWidget(SandboxWindow::Create());
	}
}
