#include "MainMenuWindow.h"

#include "CameraManager.h"
#include "CollisionSystem.h"
#include "LogSystem.h"
#include "RenderSystem.h"
#include "ResourceManager.h"

std::shared_ptr<MainMenuWindow> MainMenuWindow::Create()
{
	return std::shared_ptr<MainMenuWindow>();
}

MainMenuWindow::MainMenuWindow():
	GlobalWidget("mainMenu", eWidgetPriority::MENU, nullptr)
{
}

void MainMenuWindow::OriginRectSet()
{
	GlobalWidget::OriginRectSet();
	CAMERA.SetWorldRect(CAMERA.getActiveWindowRect());
}

void MainMenuWindow::AddedToContainer(std::shared_ptr<WidgetContainer> theWidgetContainer)
{
}

void MainMenuWindow::RemovedFromContainer(std::shared_ptr<WidgetContainer> theWidgetContainer)
{
}

void MainMenuWindow::Draw()
{
}

void MainMenuWindow::Update(float deltaTime)
{
}

void MainMenuWindow::KeyPress(const int& key)
{
	GlobalWidget::KeyPress(key);
}

void MainMenuWindow::KeyUnpress(const int& key)
{
	GlobalWidget::KeyUnpress(key);
}

void MainMenuWindow::MouseMove(const FPoint& current_pos)
{
	GlobalWidget::MouseMove(current_pos);
}

void MainMenuWindow::MousePress(const int& key)
{
	GlobalWidget::MousePress(key);
}

void MainMenuWindow::MouseUnpress(const int& key)
{
	GlobalWidget::MouseUnpress(key);
}
