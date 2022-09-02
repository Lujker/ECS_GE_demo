cmake_minimum_required(VERSION 3.19.3 FATAL_ERROR)

set (PROJ_NAME ECS_GE_demo)
project(${PROJ_NAME})

set (DEMO_SRC 
	src/main.cpp
	src/Client.cpp
	src/Systems/PhysicsSystem.cpp
	src/Systems/DemoServiceLocator.cpp
	src/Windows/SandboxWindow.cpp
	src/Windows/MainMenuWindow.cpp
	src/Entities/Knight.cpp
	src/Entities/Board.cpp
	src/Entities/GameObject.cpp
	src/Entities/Button.cpp
)

set (DEMO_INCLUDE
	src/Client.h
	src/Systems/PhysicsSystem.h
	src/Systems/DemoServiceLocator.h
	src/Windows/SandboxWindow.h
	src/Windows/MainMenuWindow.h
	src/Entities/Knight.h
	src/Entities/Board.h
	src/Entities/GameObject.h
	src/Entities/Button.h
)

set(ENGINE_INCL_DIR_demo
        engine/ECS_GE/src 
		engine/ECS_GE/src/Engine
		engine/ECS_GE/src/Managers
		engine/ECS_GE/src/Components
		engine/ECS_GE/src/Premitives
		engine/ECS_GE/src/Premitives/Render
		engine/ECS_GE/src/Systems
		engine/ECS_GE/src/Widgets
		engine/ECS_GE/external/sabmodules/rapidjson/include
		engine/ECS_GE/external/sabmodules/freetype/include
		engine/ECS_GE/external/sabmodules/assimp/include
		engine/ECS_GE/external/sabmodules/assimp/code
		engine/ECS_GE/external/sabmodules/assimp/build/include
)

include_directories(
    src
	src/Entities
	src/Windows
	src/Systems
	${ENGINE_INCL_DIR_demo}
)

add_executable(${PROJ_NAME} ${DEMO_SRC} ${DEMO_INCLUDE} ${ENGINE_INCLUDE})

target_compile_features(${PROJ_NAME} PUBLIC cxx_std_17)

add_subdirectory(engine/ECS_GE)
target_link_libraries(${PROJ_NAME} ECS_GE)

set_target_properties(${PROJ_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)
if(WIN32)
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS")
	target_link_libraries(${PROJ_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/libs/Windows/freetype.lib)
	target_link_libraries(${PROJ_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/libs/Windows/assimp-vc142-mt.lib)
	add_custom_command(TARGET ${PROJ_NAME} POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy_directory
			${CMAKE_SOURCE_DIR}/libs/Windows/dll $<TARGET_FILE_DIR:${PROJ_NAME}>) 	
endif()
set_target_properties(${PROJ_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)
add_custom_command(TARGET ${PROJ_NAME} POST_BUILD
					COMMAND ${CMAKE_COMMAND} -E copy_directory
					${CMAKE_SOURCE_DIR}/res $<TARGET_FILE_DIR:${PROJ_NAME}>/res) 