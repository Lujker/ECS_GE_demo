#pragma once
#include <string>
#include "gl_Include.h"

class RenderSystem;
class CameraManager;
class CollisionSystem;
class FileSystem;
class FontManager;
class InputManager;
class Loger;
class MoveSystem;
class ResourceManager;
class GlobalThreadPool;
class UpdateSystem;
class WidgetManager;
class ImGuiController;

#define WIDGET			Engine::getServiceLocator()->getWidgetManager()
#define UPDATE			Engine::getServiceLocator()->getUpdateSystem()
#define ASYNC			Engine::getServiceLocator()->getGlobalThreadPool()
#define RES				Engine::getServiceLocator()->getResourceManager()
#define RENDER			Engine::getServiceLocator()->getRenderSystem()
#define MOVE			Engine::getServiceLocator()->getMoveSystem()
#define LOGER			Engine::getServiceLocator()->getLoger()
#define LOG				LOGER->writeMessage
#define INPUTS			Engine::getServiceLocator()->getInputManager()
#define FONT			Engine::getServiceLocator()->getFontManager()
#define FILES			Engine::getServiceLocator()->getFileSystem()
#define COLLISION		Engine::getServiceLocator()->getCollisionSystem()
#define CAMERA			Engine::getServiceLocator()->getCameraManager()
#define UI				Engine::getServiceLocator()->getGuiController()

class ServiceLocator
{
public:
	ServiceLocator() = default;
	virtual ~ServiceLocator();
	virtual void init(const std::string& init_path);
	virtual void postInit(GLFWwindow* main_window);
	[[nodiscard]] WidgetManager*		getWidgetManager() const { return p_widget_manager; }
	[[nodiscard]] UpdateSystem*			getUpdateSystem() const { return p_update_system; }
	[[nodiscard]] GlobalThreadPool*		getGlobalThreadPool() const { return p_global_thread_pool; }
	[[nodiscard]] ResourceManager*		getResourceManager() const { return p_resource_manager; }
	[[nodiscard]] RenderSystem*			getRenderSystem() const { return p_render_system; }
	[[nodiscard]] MoveSystem*			getMoveSystem() const { return p_move_system; }
	[[nodiscard]] Loger*				getLoger() const { return p_loger; }
	[[nodiscard]] InputManager*			getInputManager() const { return p_input_manager; }
	[[nodiscard]] FontManager*			getFontManager() const { return p_font_manager; }
	[[nodiscard]] FileSystem*			getFileSystem() const { return p_file_system; }
	[[nodiscard]] CollisionSystem*		getCollisionSystem() const { return p_collision_system; }
	[[nodiscard]] CameraManager*		getCameraManager() const { return p_camera_manager; }
	[[nodiscard]] ImGuiController*		getGuiController() const { return p_gui_controller; }

	void				setWidgetManager(WidgetManager* widget_manager) { p_widget_manager = widget_manager; }
	void				setUpdateSystem(UpdateSystem* update_system) { p_update_system = update_system; }
	void				setGlobalThreadPool(GlobalThreadPool* global_thread_pool) { p_global_thread_pool = global_thread_pool; }
	void				setResourceManager(ResourceManager* resource_manager) { p_resource_manager = resource_manager; }
	void				setRenderSystem(RenderSystem* render_system) { p_render_system = render_system; }
	void				setMoveSystem(MoveSystem* move_system) { p_move_system = move_system; }
	void				setLoger(Loger* loger) { p_loger = loger; }
	void				setInputManager(InputManager* input_manager) { p_input_manager = input_manager; }
	void				setFontManager(FontManager* font_manager) { p_font_manager = font_manager; }
	void				setFileSystem(FileSystem* file_system) { p_file_system = file_system; }
	void				setCollisionSystem(CollisionSystem* collision_system) { p_collision_system = collision_system; }
	void				setCameraManager(CameraManager* camera_manager) { p_camera_manager = camera_manager; }
	void				setGuiController(ImGuiController* gui_controller) { p_gui_controller = gui_controller; }

	ServiceLocator(const ServiceLocator&) = delete;
	ServiceLocator(ServiceLocator&&) = delete;
	ServiceLocator& operator=(const ServiceLocator&) = delete;
	ServiceLocator& operator=(ServiceLocator&&) = delete;

protected:
	void destroyServiceLocator();
	WidgetManager*		p_widget_manager = nullptr;
	UpdateSystem*		p_update_system = nullptr;
	GlobalThreadPool*	p_global_thread_pool = nullptr;
	ResourceManager*	p_resource_manager = nullptr;
	RenderSystem*		p_render_system = nullptr;
	MoveSystem*			p_move_system = nullptr;
	Loger*				p_loger = nullptr;
	InputManager*		p_input_manager = nullptr;
	FontManager*		p_font_manager = nullptr;
	FileSystem*			p_file_system = nullptr;
	CollisionSystem*	p_collision_system = nullptr;
	CameraManager*		p_camera_manager = nullptr;
	ImGuiController*	p_gui_controller = nullptr;
};