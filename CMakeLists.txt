cmake_minimum_required(VERSION 3.19.3 FATAL_ERROR)

set (PROJ_NAME ECS_GE_demo)
project(${PROJ_NAME})

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
)

include_directories(
    	src
	${ENGINE_INCL_DIR_demo}
)

add_executable(${PROJ_NAME} src/main.cpp ${ENGINE_INCLUDE})

target_compile_features(${PROJ_NAME} PUBLIC cxx_std_17)

add_subdirectory(engine/ECS_GE)
target_link_libraries(${PROJ_NAME} ECS_GE)

set_target_properties(${PROJ_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)
if(WIN32)
	set(ENGINE_DLL
		${CMAKE_CURRENT_SOURCE_DIR}/libs/Windows/freetype.dll
	)
	target_link_libraries(${PROJ_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/libs/Windows/freetype.lib)
	foreach(file_i ${ENGINE_DLL})
		add_custom_command(TARGET ${PROJ_NAME} POST_BUILD
			COMMAND ${CMAKE_COMMAND}
			ARGS -E copy ${file_i} $<TARGET_FILE_DIR:${PROJ_NAME}>) 	
	endforeach()
endif()
set_target_properties(${PROJ_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)
add_custom_command(TARGET ${PROJ_NAME} POST_BUILD
					COMMAND ${CMAKE_COMMAND} -E copy_directory
					${CMAKE_SOURCE_DIR}/res $<TARGET_FILE_DIR:${PROJ_NAME}>/res) 