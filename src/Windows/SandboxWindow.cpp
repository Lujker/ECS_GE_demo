#include "SandboxWindow.h"

#include "CameraManager.h"
#include "CollisionSystem.h"
#include "LogSystem.h"
#include "PhysicsSystem.h"
#include "RenderSystem.h"
#include "ResourceManager.h"

std::shared_ptr<SandboxWindow> SandboxWindow::Create()
{
	return std::make_shared<SandboxWindow>();
}

SandboxWindow::SandboxWindow():
	GlobalWidget("SandboxWindow", eWidgetPriority::DEFAULT, nullptr)
{}

void SandboxWindow::OriginRectSet()
{
	GlobalWidget::OriginRectSet();
}

void SandboxWindow::AddedToContainer(std::shared_ptr<WidgetContainer> theWidgetContainer)
{
    const CollisionComponent col_form{ std::vector<FPoint>{ {30, 0},{15, 25}, {25, 30},{35, 25} ,{35, 10} }, false, 2 };
    
    m_knight = std::make_shared<Knight>("player", PositionComponent{ 350, 100, 0, 0 }, CollisionComponent{ 120, 80, false }, CollisionComponent{ 120, 80, true } );
    m_board = std::make_shared<Board>("board", PositionComponent{ mOrigin.mX, mOrigin.mY, static_cast<float>(CAMERA.getNearLayer() + 1) },
        CollisionComponent{ static_cast<float>(mOrigin.mWidth), static_cast<float>(mOrigin.mHeight) }, CollisionComponent{ static_cast<float>(mOrigin.mWidth), static_cast<float>(55) });

    if (m_knight)
    {
        m_knight->Init();
        PHYSICS.Registrate(m_knight);
    }
	if (m_board)
	{
        m_board->Init();
        PHYSICS.Registrate(m_board);
	}

    
    if (COLLISION.intersect2D(col_form, { 300,300 }, col_form, { 320,300 }))
        LOG("Intersect2D!");
    else
        LOG("Not intersect2D");
}

void SandboxWindow::RemovedFromContainer(std::shared_ptr<WidgetContainer> theWidgetContainer)
{
}

void SandboxWindow::Draw()
{
    if (m_board)
        m_board->Draw();
    if (m_knight)
		m_knight->Draw();
}

void SandboxWindow::Update(float deltaTime)
{
    if (m_board)
        m_board->Update(deltaTime);
    if (m_knight)
		m_knight->Update(deltaTime);
}

void SandboxWindow::KeyPress(const int& key)
{
    if (m_knight)
		m_knight->KeyPress(key);
}

void SandboxWindow::KeyUnpress(const int& key)
{
    if (m_knight)
		m_knight->KeyUnpress(key);
}
