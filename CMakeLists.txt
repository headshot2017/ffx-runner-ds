cmake_minimum_required(VERSION 3.5)

project(ffx-runner-ds)

file(GLOB SOURCES
	source/*.*
	source/engine/sdl2/*.*
	source/engine/*.*
	source/game/entities/*.*
	source/game/screens/*.*
	source/game/mesh/gl/*.*
	source/game/mesh/*.*
	source/game/*.*
	source/thirdparty/*.*
	source/thirdparty/box3d/src/*.*
)

option(CONSOLE "Show console on Windows" ON)

if (DEBUG)
	set(CMAKE_BUILD_TYPE Debug)
endif()

find_package(SDL2 REQUIRED)
find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

add_executable(ffx-runner-ds ${SOURCES})
target_include_directories(ffx-runner-ds PRIVATE
	${PROJECT_SOURCE_DIR}/source
	${PROJECT_SOURCE_DIR}/source/thirdparty
	${PROJECT_SOURCE_DIR}/source/thirdparty/box3d/include
)

target_link_libraries(ffx-runner-ds GLEW::GLEW GLUT::GLUT SDL2::SDL2 OpenGL::GL OpenGL::GLU)
#target_link_options(ffx-runner-ds PRIVATE -static-libstdc++)

if (WIN32)
	target_link_libraries(ffx-runner-ds opengl32 ddraw dinput8 dxguid dxerr8 user32 gdi32 winmm)

	if(NOT CONSOLE)
		target_link_options(ffx-runner-ds PRIVATE -mwindows)
	endif()
elseif(APPLE)
	set(CMAKE_MACOSX_RPATH 1)
	set_target_properties(ffx-runner-ds PROPERTIES
		MACOSX_RPATH 1
		BUILD_WITH_INSTALL_RPATH 1
		INSTALL_RPATH "@loader_path/../Frameworks;/Library/Frameworks")
else()
	target_link_libraries(ffx-runner-ds m)
endif()

#add_custom_command(TARGET ffx-runner-ds POST_BUILD
#                   COMMAND ${CMAKE_COMMAND} -E copy_directory
#                       ${PROJECT_SOURCE_DIR}/data $<TARGET_FILE_DIR:ffx-runner-ds>/data)
