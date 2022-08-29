#include "ServiceLocator.h"

#include "WidgetsManager.h"
#include "InputManager.h"
#include "UpdateSystem.h"
#include "RenderSystem.h"
#include "ResourceManager.h"
#include "LogSystem.h"
#include "MoveSystem.h"
#include "ThreadPool.h"
#include "FileSystemManager.h"
#include "FontManager.h"
#include "CollisionSystem.h"
#include "CameraManager.h"
#include "ImGuiController.h"

ServiceLocator::~ServiceLocator()
{
	destroyServiceLocator();
}

void ServiceLocator::init(const std::string& init_path)
{
	destroyServiceLocator();
	p_widget_manager = new WidgetManager;
	p_update_system = new UpdateSystem;
	p_global_thread_pool = new GlobalThreadPool;
	p_resource_manager = new ResourceManager;
	p_render_system = new RenderSystem;
	p_move_system = new MoveSystem;
	p_loger = new Loger;
	p_input_manager = new InputManager;
	p_font_manager = new FontManager;
	p_file_system = new FileSystem;
	p_collision_system = new CollisionSystem;
	p_camera_manager = new CameraManager;
	p_gui_controller = new ImGuiController;

	p_camera_manager->setActiveWindowSize(1310, 768);
	p_loger->init(init_path);
	p_file_system->setExecutablePath(init_path);
	p_resource_manager->setExecutablePath(init_path);
	p_font_manager->init(init_path);
}

void ServiceLocator::postInit(GLFWwindow* main_window)
{
	if (p_gui_controller)
		p_gui_controller->init(main_window);
}

void ServiceLocator::destroyServiceLocator()
{
	//!Для выделения и освобождения нормального нужно написать свой аллокатор
	delete p_widget_manager;
	delete p_update_system;
	delete p_global_thread_pool;
	delete p_resource_manager;
	delete p_render_system;
	delete p_move_system;
	delete p_loger;
	delete p_input_manager;
	delete p_font_manager;
	delete p_file_system;
	delete p_collision_system;
	delete p_camera_manager;
	delete p_gui_controller;
}
