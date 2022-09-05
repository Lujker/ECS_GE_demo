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
#include "SkyBox.h"
#include "Sphere.h"

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
    std::vector < std::string> TexturesSkyBox
    {
       "res/images/skybox/right.jpg",
        "res/images/skybox/left.jpg",
        "res/images/skybox/top.jpg",
        "res/images/skybox/bottom.jpg",
        "res/images/skybox/front.jpg",
        "res/images/skybox/back.jpg",
    };
    sky_box = std::make_shared<RenderEngine::SkyBox>(RES->loadTexture3("SkyBoxTexture", TexturesSkyBox));
    model = RES->loadModel("res/models/Wolf/Wolf.obj", "res/models/Wolf/Wolf.obj");

    auto cube = std::make_shared<RenderEngine::Cube>(
        RES->loadTexture("res/images/container2.png", "res/images/container2.png"), 
        RES->loadTexture("res/images/container2_specular.png", "res/images/container2_specular.png"));
    m_cubs.emplace_back(CubeObject{ cube });
    m_cubs.emplace_back(CubeObject{ cube, {-100, 0, 200} });

    auto light = std::make_shared<RenderEngine::PointLight>();
    light->lightCube.setColor({ 1.0f, 0.1f, 0.5f });
    light->position = { 0, 0, 230 };
    light->size = { 10.f, 10.f, 10.f };
    light->ambient = { 0.2f, 0.2f, 0.2f };
    light->diffuse = { 0.5f, 0.5f, 0.5f };
    light->specular = { 1.0f, 1.0f, 1.0f };

    auto light_1 = std::make_shared<RenderEngine::PointLight>();
    light_1->lightCube.setColor({ 0.5f, 0.1f, 1.0f });
    light_1->position = { 50, 20, 100 };
    light_1->size = { 10.f, 10.f, 10.f };
    light_1->ambient = { 0.2f, 0.2f, 0.2f };
    light_1->diffuse = { 0.5f, 0.5f, 0.5f };
    light_1->specular = { 1.0f, 1.0f, 1.0f };

    auto worldLight = std::make_shared<RenderEngine::DirectionLight>();
    worldLight->lightCube.setColor({ 1.0f, 1.0f, 1.0f });
    worldLight->direction = {1.0f, -1.0f, -0.2f };
    worldLight->size = { 10.f, 10.f, 10.f };
    worldLight->ambient = { 1.0f, 1.0f, 1.0f };
    worldLight->diffuse = { 1.0f, 1.0f, 1.0f };
    worldLight->specular = { 1.0f, 1.0f, 1.0f };

    sphere = std::make_shared<RenderEngine::Sphere>(RES->getTexture("res/images/container2.png"));

    LIGHT->pushLight("first", light);
    LIGHT->pushLight("second", light_1);
    LIGHT->pushLight("world", worldLight);

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
    if (model)
        RENDER->Render(model, { 100.f, 0.f, 0.f }, { 25.f, 25.f, 25.f });
    if (sky_box)
        RENDER->Render(sky_box);
    if (sphere)
        RENDER->Render(sphere, { 200.f, 0.f ,0.f }, { 25.f, 25.f, 25.f });
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
