#include "SandboxWindow.h"

#include "Board.h"
#include "CameraManager.h"
#include "DemoServiceLocator.h"
#include "CollisionSystem.h"
#include "Knight.h"
#include "LogSystem.h"
#include "MainMenuWindow.h"
#include "PhysicsSystem.h"
#include "ResourceManager.h"
#include "WidgetsManager.h"
#include "LightManager.h"

std::shared_ptr<SandboxWindow> SandboxWindow::Create()
{
	return std::make_shared<SandboxWindow>();
}

SandboxWindow::SandboxWindow() :
    GlobalWidget("SandboxWindow", eWidgetPriority::DEFAULT, nullptr)
{}

void SandboxWindow::OriginRectSet()
{
	GlobalWidget::OriginRectSet();
    CAMERA->SetWorldRect({ 0,0,3000, 2000 });
}

void SandboxWindow::AddedToContainer(SharedWidgetContainer theWidgetContainer)
{
 //   const CollisionComponent col_form{ std::vector<FPoint>{ {30, 0},{15, 25}, {25, 30},{35, 25} ,{35, 10} }, false, 2 };
 //   m_objects.push_back(std::make_shared<Knight>("player", PositionComponent{ 350., 100., 0., 0. }, 
 //       CollisionComponent{ 120., 80., 0., 0., false }, CollisionComponent{ 120., 80., 0., 0.,  true }));

	//m_objects.push_back(std::make_shared<Board>("board", PositionComponent{ mOrigin.mX, mOrigin.mY, static_cast<float>(1) },
	//	       CollisionComponent{ static_cast<float>(mOrigin.mWidth), static_cast<float>(mOrigin.mHeight) }, CollisionComponent{ static_cast<float>(mOrigin.mWidth), static_cast<float>(60) }, "res/images/Layer_0001_8.png"));

	//m_objects.push_back(std::make_shared<Board>("board_1", PositionComponent{ mOrigin.mX, mOrigin.mY, static_cast<float>(1) },
 //              CollisionComponent{ static_cast<float>(100), static_cast<float>(200) }, CollisionComponent{ static_cast<float>(100), static_cast<float>(200) }));

 //   m_objects.push_back(std::make_shared<Board>("board_2", PositionComponent{ mOrigin.mX + mOrigin.mWidth/2, mOrigin.mY, static_cast<float>(1) },
 //       CollisionComponent{ static_cast<float>(100), static_cast<float>(200) }, CollisionComponent{ static_cast<float>(100), static_cast<float>(200) }));

 //   m_objects.push_back(std::make_shared<Board>("board_3", PositionComponent{ mOrigin.mX, mOrigin.mY + mOrigin.mHeight/2, static_cast<float>(1) },
 //       CollisionComponent{ static_cast<float>(1000), static_cast<float>(200) }, CollisionComponent{ static_cast<float>(1000), static_cast<float>(200) }));
 //   for (const auto& it : m_objects)
 //   {
 //       it->Init();
 //       PHYSICS->Registrate(it);
 //   }

    auto cube = std::make_shared<RenderEngine::Cube>(RES->loadTexture("res/images/container2.png", "res/images/container2.png"), RES->loadTexture("res/images/container2_specular.png", "res/images/container2_specular.png"));
    m_cubs.emplace_back(CubeObject{ cube });
    m_cubs.emplace_back(CubeObject{ cube, {-100, 0, 200} });

    auto light = std::make_shared< RenderEngine::Light>();
    light->position = { 100, 100, 200 };
    light->size = { 10.f, 10.f, 10.f };
    light->ambient = { 0.2f, 0.2f, 0.2f };
    light->diffuse = { 0.5f, 0.5f, 0.5f };
    light->specular = { 1.0f, 1.0f, 1.0f };

    LIGHT->pushLight("first", light);

    CAMERA->SetPerspectiveProj();
}

void SandboxWindow::RemovedFromContainer(SharedWidgetContainer theWidgetContainer)
{
    PHYSICS->Clear();
    LIGHT->clearLights();
}

void SandboxWindow::Draw()
{
    for (const auto& it : m_objects)
    {
        it->Draw();
    }
    for (const auto& it : m_cubs)
        RENDER->Render(it.cub, it.cub_pos, it.cub_size);
}

void SandboxWindow::Update(float deltaTime)
{
    for (const auto& it : m_objects)
    {
        it->Update(deltaTime);
    }
}

void SandboxWindow::KeyPress(const int& key)
{
    for (const auto& it : m_objects)
    {
        it->KeyPress(key);
    }
}

void SandboxWindow::KeyUnpress(const int& key)
{
    if (key == GLFW_KEY_ESCAPE)
    {
        WIDGET->SetNextWidget(MainMenuWindow::Create());
        //WIDGET->Close();
    }
    for (const auto& it : m_objects)
    {
        it->KeyUnpress(key);
    }
}
