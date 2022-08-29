cmake_minimum_required(VERSION 3.19.3 FATAL_ERROR)

set (PROJ_NAME ECS_GE)
project(${PROJ_NAME})

set(ENGINE_SRC
	src/Engine/Engine.cpp
	src/Managers/FileSystemManager.cpp
	src/Managers/ResourceManager.cpp
	src/Managers/CameraManager.cpp
	src/Managers/FontManager.cpp
	src/Managers/InputManager.cpp
	src/Managers/WidgetsManager.cpp
	src/Managers/ServiceLocator.cpp
	src/Premitives/Animator.cpp
	src/Premitives/Shape.cpp  
	src/Premitives/Timer.cpp
	src/Premitives/Render/IndexBuffer.cpp
	src/Premitives/Render/VertexArray.cpp 
	src/Premitives/Render/VertexBufferLayout.cpp
	src/Premitives/Render/ShaderProgram.cpp 
	src/Premitives/Render/VertexBuffer.cpp  
	src/Premitives/Render/texture2D.cpp
	src/Components/Atlas2D.cpp
	src/Components/Collision.cpp
	src/Components/Health.cpp
	src/Components/Image.cpp
	src/Components/Cube.cpp
	src/Components/Move.cpp
	src/Components/Position.cpp
	src/Components/Collor.cpp
	src/Components/Sprite.cpp
	src/Components/DisplayString.cpp
	src/Systems/RenderSystem.cpp
	src/Systems/UpdateSystem.cpp
	src/Systems/CollisionSystem.cpp
	src/Systems/MoveSystem.cpp
	src/Systems/LogSystem.cpp 
	src/Systems/ThreadPool.cpp 
	src/Systems/ImGuiController.cpp
	src/Widgets/Widget.cpp
	src/Widgets/GlobalWidget.cpp
	src/Widgets/WidgetContainer.cpp	
)

set(ENGINE_INCLUDE
	src/gl_include.h
	src/Engine/Engine.h
	src/Managers/FileSystemManager.h
	src/Managers/ResourceManager.h
	src/Managers/CameraManager.h
	src/Managers/FontManager.h
	src/Managers/InputManager.h
	src/Managers/WidgetsManager.h
	src/Managers/ServiceLocator.h
	src/Managers/stb_image.h
	src/Premitives/Animator.h
	src/Premitives/Point.h
	src/Premitives/Vector.h
	src/Premitives/Rect.h  
	src/Premitives/Shape.h
	src/Premitives/Timer.h
	src/Premitives/FunctionWrapper.h
	src/Components/DisplayString.h
	src/Premitives/Render/IndexBuffer.h  
	src/Premitives/Render/VertexArray.h  
	src/Premitives/Render/VertexBufferLayout.h 
	src/Premitives/Render/ShaderProgram.h 
	src/Premitives/Render/VertexBuffer.h    
	src/Premitives/Render/texture2D.h
	src/Components/IComponent.h
	src/Components/Atlas2D.h
	src/Components/Collision.h 
	src/Components/Health.h  
	src/Components/Image.h 
	src/Components/Cube.h
	src/Components/Move.h
	src/Components/Position.h 
	src/Components/Collor.h 
	src/Components/Sprite.h
	src/Components/DisplayString.h
	src/Systems/ISystem.h
	src/Systems/RenderSystem.h
	src/Systems/UpdateSystem.h
	src/Systems/CollisionSystem.h
	src/Systems/MoveSystem.h
	src/Systems/LogSystem.h
	src/Systems/ThreadPool.h
	src/Systems/ImGuiController.h
	src/Widgets/Widget.h
	src/Widgets/GlobalWidget.h
	src/Widgets/WidgetContainer.h
)

set(IMGUI_SRC
	external/sabmodules/imgui/imgui.cpp
	external/sabmodules/imgui/backends/imgui_impl_opengl3.cpp 
	external/sabmodules/imgui/backends/imgui_impl_glfw.cpp
	external/sabmodules/imgui/imgui_demo.cpp
	external/sabmodules/imgui/imgui_draw.cpp
	external/sabmodules/imgui/imgui_tables.cpp
	external/sabmodules/imgui/imgui_widgets.cpp
)

set(IMGUI_INCLUDES
	external/sabmodules/imgui/imgui.h
	external/sabmodules/imgui/backends/imgui_impl_opengl3.h 
	external/sabmodules/imgui/backends/imgui_impl_glfw.h
)

add_library(${PROJ_NAME} STATIC 	
		${ENGINE_SRC}
		${ENGINE_INCLUDE})

include_directories(
	src 
	src/Engine
	src/Managers
	src/Components
	src/Premitives
	src/Premitives/Render
	src/Systems
	src/Widgets
	external/sabmodules/rapidjson/include
	external/sabmodules/freetype/include
	external/sabmodules/assimp/include/assimp
)

target_compile_features(${PROJ_NAME} PUBLIC cxx_std_17)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

add_subdirectory(external/sabmodules/glfw)
target_link_libraries(${PROJ_NAME} glfw)

add_subdirectory(external/glad)
target_link_libraries(${PROJ_NAME} glad)

add_subdirectory(external/sabmodules/glm)
target_link_libraries(${PROJ_NAME} glm)

add_subdirectory(external/sabmodules/assimp)
target_link_libraries(${PROJ_NAME} assimp)

add_library(
	ImGUI STATIC
	${IMGUI_INCLUDES} 
	${IMGUI_SRC}
)

target_include_directories(ImGUI PUBLIC external/sabmodules/imgui)
target_link_libraries(ImGUI PRIVATE glad glfw)
target_link_libraries(${PROJ_NAME} ImGUI)