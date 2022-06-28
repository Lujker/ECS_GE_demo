#include "SandboxWindow.h"

#include "CameraManager.h"
#include "CollisionSystem.h"
#include "DisplayString.h"
#include "LogSystem.h"
#include "RenderSystem.h"
#include "ResourceManager.h"
#include "Sprite.h"
#include "UpdateSystem.h"

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
    pos_sprite = { 350, 100 };
    size_sprite = { 120, 80, false };
    pos_string = { 300,300, 2 };

    const CollisionComponent col_form{ std::vector<FPoint>{ {30, 0},{15, 25}, {25, 30},{35, 25} ,{35, 10} }, false, 2 };
    m_bg = RES.getSharedImage("res\\images\\Layer_0005_5.png");
    m_knight = RES.getSprite("res/sprites/test/Kinght");
    m_string = std::make_shared<DisplayString>("Can't");

    //m_knight->setAnimation("idle");
    for (auto i : m_knight->getAnimationsName())
        std::cout << i << std::endl;

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
    RENDER.Render(m_bg, { mOrigin.mX, mOrigin.mY, static_cast<float>(CAMERA.getNearLayer() - 1) }, 
        CollisionComponent{ static_cast<float>(mOrigin.mWidth), static_cast<float>(mOrigin.mHeight) });
    RENDER.Render(m_knight, pos_sprite, size_sprite);
}

void SandboxWindow::Update(float deltaTime)
{
    UPDATE.Update(m_knight);
}

void SandboxWindow::KeyPress(const int& key)
{
}

void SandboxWindow::KeyUnpress(const int& key)
{
}
