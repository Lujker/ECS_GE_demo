cmake_minimum_required(VERSION 3.19.3 FATAL_ERROR)

set (PROJ_NAME ECS_GE_demo)
project(${PROJ_NAME} )

add_executable(${PROJ_NAME} 
	src/main.cpp

	src/Managers/FileSystemManager.cpp
	src/Managers/FileSystemManager.h
	src/Managers/ResourceManager.cpp
	src/Managers/ResourceManager.h
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
	src/Components/Size.cpp
	src/Components/Size.h 
	src/Components/Sprite.cpp
	src/Components/Sprite.h

	src/Systems/RenderSystem.h
	src/Systems/RenderSystem.cpp
	src/Systems/CollisionSystem.h
	src/Systems/CollisionSystem.cpp
)

include_directories(
	src 
	src/Managers
	src/Premitives
	src/Premitives/Render
	src/Components
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

find_package(RapidJSON)
include_directories(external/sabmodules/rapidjson/include)


set_target_properties(${PROJ_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)

add_custom_command(TARGET ${PROJ_NAME} POST_BUILD
					COMMAND ${CMAKE_COMMAND} -E copy_directory
					${CMAKE_SOURCE_DIR}/res $<TARGET_FILE_DIR:${PROJ_NAME}>/res) 