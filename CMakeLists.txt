cmake_minimum_required(VERSION 3.19.3 FATAL_ERROR)

set (PROJ_NAME ECS_GE_demo)
project(${PROJ_NAME})

add_executable(${PROJ_NAME} 
	src/main.cpp
	src/gl_include.h
	
	src/Managers/FileSystemManager.cpp
	src/Managers/FileSystemManager.h
	src/Managers/ResourceManager.cpp
	src/Managers/ResourceManager.h
	src/Managers/CameraManager.cpp
	src/Managers/CameraManager.h
	src/Managers/stb_image.h

	src/Premitives/Animator.cpp
	src/Premitives/Animator.h
	src/Premitives/Point.h
	src/Premitives/Vector.h
	src/Premitives/Rect.h  
	src/Premitives/Shape.h
	src/Premitives/Shape.cpp  
	src/Premitives/Timer.h
	src/Premitives/Timer.cpp

	src/Premitives/Render/IndexBuffer.h  
	src/Premitives/Render/IndexBuffer.cpp
	src/Premitives/Render/VertexArray.h  
	src/Premitives/Render/VertexArray.cpp 
	src/Premitives/Render/VertexBufferLayout.h 
	src/Premitives/Render/VertexBufferLayout.cpp
	src/Premitives/Render/ShaderProgram.h 
	src/Premitives/Render/ShaderProgram.cpp 
	src/Premitives/Render/VertexBuffer.h    
	src/Premitives/Render/VertexBuffer.cpp  
	src/Premitives/Render/texture2D.h
	src/Premitives/Render/texture2D.cpp

	src/Components/IComponent.h
	src/Components/Atlas2D.cpp
	src/Components/Atlas2D.h
	src/Components/Collision.cpp
	src/Components/Collision.h 
	src/Components/Health.cpp
	src/Components/Health.h  
	src/Components/Image.cpp
	src/Components/Image.h 
	src/Components/Move.cpp
	src/Components/Move.h
	src/Components/Position.cpp
	src/Components/Position.h 
	src/Components/Sprite.cpp
	src/Components/Sprite.h

	src/Systems/ISystem.h
	src/Systems/RenderSystem.h
	src/Systems/RenderSystem.cpp
	src/Systems/UpdateSystem.h
	src/Systems/UpdateSystem.cpp
	src/Systems/CollisionSystem.h
	src/Systems/CollisionSystem.cpp
	src/Systems/MoveSystem.h
	src/Systems/MoveSystem.cpp

	src/Widgets/Widget.cpp
	src/Widgets/Widget.h
)

include_directories(
	src 
	src/Managers
	src/Premitives
	src/Premitives/Render
	src/Components
	external/sabmodules/rapidjson/include
	external/freetype/include
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

set( ENGINE_DLL
	${CMAKE_CURRENT_SOURCE_DIR}/libs/Windows/freetype.dll
)

if(WIN32)
  target_link_libraries(${PROJ_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/libs/Windows/freetype.lib)
endif()

set_target_properties(${PROJ_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)
add_custom_command(TARGET ${PROJ_NAME} POST_BUILD
					COMMAND ${CMAKE_COMMAND} -E copy_directory
					${CMAKE_SOURCE_DIR}/res $<TARGET_FILE_DIR:${PROJ_NAME}>/res) 
	
foreach(file_i ${ENGINE_DLL})
	add_custom_command(TARGET ${PROJ_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND}
		ARGS -E copy ${file_i} $<TARGET_FILE_DIR:${PROJ_NAME}>) 	
endforeach()

