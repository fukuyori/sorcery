# Project Definition
cmake_minimum_required(VERSION 3.22...3.22)
set(CMAKE_CXX_STANDARD 23)
project(sorcery VERSION 0.1.0)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Prerequisites
set(CMAKE_WARN_DEPRECATED OFF CACHE BOOL "" FORCE)
set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)
find_package(Threads REQUIRED)
find_package(SDL2 REQUIRED)
find_package(SDL2_image CONFIG QUIET)
if(NOT SDL2_image_FOUND)
	find_package(SDL2_image REQUIRED)
endif()
find_package(OpenGL REQUIRED)
find_package(SQLite3 REQUIRED)
find_package(PkgConfig REQUIRED)
find_package(Freetype REQUIRED)
find_package(GLEW REQUIRED)
find_package(jsoncpp CONFIG QUIET)
if(NOT jsoncpp_FOUND)
	find_package(jsoncpp REQUIRED)
endif()

if(TARGET GLEW::GLEW)
	set(SORCERY_GLEW_TARGET GLEW::GLEW)
else()
	set(SORCERY_GLEW_TARGET GLEW)
endif()

if(TARGET SDL2::SDL2)
	set(SORCERY_SDL2_TARGETS SDL2::SDL2)
elseif(TARGET SDL2::SDL2-static)
	set(SORCERY_SDL2_TARGETS SDL2::SDL2-static)
else()
	set(SORCERY_SDL2_TARGETS ${SDL2_LIBRARIES})
endif()

if(TARGET SDL2_image::SDL2_image)
	list(APPEND SORCERY_SDL2_TARGETS SDL2_image::SDL2_image)
elseif(TARGET SDL2_image::SDL2_image-static)
	list(APPEND SORCERY_SDL2_TARGETS SDL2_image::SDL2_image-static)
else()
	list(APPEND SORCERY_SDL2_TARGETS ${SDL2_IMAGE_LIBRARIES})
endif()

if(TARGET JsonCpp::JsonCpp)
	set(SORCERY_JSONCPP_TARGET JsonCpp::JsonCpp)
elseif(TARGET jsoncpp_lib)
	set(SORCERY_JSONCPP_TARGET jsoncpp_lib)
else()
	set(SORCERY_JSONCPP_TARGET jsoncpp)
endif()

if(TARGET SQLite::SQLite3)
	set(SORCERY_SQLITE_TARGET SQLite::SQLite3)
else()
	set(SORCERY_SQLITE_TARGET sqlite3)
endif()

set(SORCERY_PLATFORM_LIBRARIES)
set(SORCERY_UUID_LIBRARIES)

if(WIN32)
	list(APPEND SORCERY_PLATFORM_LIBRARIES ole32)
	add_compile_definitions(
		SDL_MAIN_HANDLED
		WIN32_LEAN_AND_MEAN
		NOMINMAX
		GLM_ENABLE_EXPERIMENTAL
	)
elseif(UNIX)
	list(APPEND SORCERY_PLATFORM_LIBRARIES dl backtrace)
	list(APPEND SORCERY_UUID_LIBRARIES uuid)
	if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
		list(APPEND SORCERY_PLATFORM_LIBRARIES stdc++fs stdc++exp)
	endif()
endif()

set(SORCERY_THIRD_PARTY_INCLUDE_DIRS
	${CMAKE_SOURCE_DIR}/ext
	${CMAKE_SOURCE_DIR}/ext/cereal/include
	${CMAKE_SOURCE_DIR}/ext/glm
	${CMAKE_SOURCE_DIR}/ext/magic_enum/include
	${CMAKE_SOURCE_DIR}/ext/simpleini
	${CMAKE_SOURCE_DIR}/ext/sqlite_modern_cpp/hdr
)

# Zep
add_definitions(-DZEP_USE_SDL)
add_definitions(-DZEP_FEATURE_CPP_FILE_SYSTEM)
add_definitions(-DZEP_SINGLE_HEADER=1)

# ffmpeg
pkg_check_modules(LIBAV REQUIRED IMPORTED_TARGET
	libavformat
	libavcodec
	libswresample
	libswscale
	libavutil
)

if(NOT EXISTS "${CMAKE_SOURCE_DIR}/ext/imgui/imgui.cpp")
	message(FATAL_ERROR
		"Missing bundled third-party sources under ext/. "
		"Run scripts/restore-ext.ps1 or restore the ext directory before "
		"building Sorcery.")
endif()

# Imgui Library
add_library(dear_imgui EXCLUDE_FROM_ALL)

target_sources(dear_imgui PRIVATE
	ext/imgui/imgui.cpp
	ext/imgui/imgui_demo.cpp
	ext/imgui/imgui_draw.cpp
	ext/imgui/imgui_tables.cpp
	ext/imgui/imgui_widgets.cpp
	ext/imgui/backends/imgui_impl_sdl2.cpp
	ext/imgui/backends/imgui_impl_opengl3.cpp
	ext/imgui/misc/cpp/imgui_stdlib.cpp
	ext/imgui_toggle/imgui_toggle.cpp
	ext/imgui_toggle/imgui_toggle_palette.cpp
	ext/imgui_toggle/imgui_toggle_presets.cpp
	ext/imgui_toggle/imgui_toggle_renderer.cpp
	ext/imgui/misc/freetype/imgui_freetype.cpp
)

target_include_directories(dear_imgui PUBLIC
	${SDL2_INCLUDE_DIRS}
	${SDL2_IMAGE_INCLUDE_DIRS}
	${OPENGL_INCLUDE_DIR}
	${GLUT_INCLUDE_DIRS}
	${SORCERY_THIRD_PARTY_INCLUDE_DIRS}
	${CMAKE_SOURCE_DIR}/ext/imgui
	${CMAKE_SOURCE_DIR}/ext/imgui/backends
	${CMAKE_SOURCE_DIR}/ext/imgui_toggle
	${CMAKE_SOURCE_DIR}/ext/imgui_sugar
	${CMAKE_SOURCE_DIR}/ext/imgui/misc/freetype
)

target_link_libraries(dear_imgui PRIVATE
	Freetype::Freetype
	${SORCERY_SDL2_TARGETS}
)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
	target_compile_options(dear_imgui PRIVATE
		-Wno-switch-default
		-Wno-suggest-attribute=format
	)
endif()

target_compile_definitions(dear_imgui PRIVATE
	IMGUI_ENABLE_FREETYPE
)

add_library(sorcery_warnings INTERFACE)

if(MSVC)
	target_compile_options(sorcery_warnings INTERFACE
		/W3
		/permissive-
	)
else()
	target_compile_options(sorcery_warnings INTERFACE
		-Wall
		-Wextra
		-Wpedantic
		-Wno-narrowing
		-Wno-unused-parameter
	)
endif()

# Main Source Files
include(src/core/CMakeLists.txt)
include(src/engine/CMakeLists.txt)
include(src/frontend/CMakeLists.txt)
include(src/gui/CMakeLists.txt)
include(src/modules/CMakeLists.txt)
include(src/resources/CMakeLists.txt)
include(src/types/CMakeLists.txt)
add_executable(${PROJECT_NAME}
	src/sorcery.cpp
	${sorcery_SOURCES})

# Include Directories
target_include_directories(${PROJECT_NAME} PRIVATE
	${SDL2_INCLUDE_DIRS}
	${SDL2_IMAGE_INCLUDE_DIRS}
	${OPENGL_INCLUDE_DIR}
	${GLUT_INCLUDE_DIRS}
	${SORCERY_THIRD_PARTY_INCLUDE_DIRS}
	${CMAKE_SOURCE_DIR}/ext/imgui
	${CMAKE_SOURCE_DIR}/inc)

# Compiler Options
if(MSVC)
	target_compile_options(${PROJECT_NAME} PRIVATE
		/W3
		/permissive-
	)
else()
	target_compile_options(${PROJECT_NAME} PRIVATE
		-Wall
		-Wextra
		-Wpedantic
		-Wunreachable-code
		-Wunused
		-Wignored-qualifiers
		-Wcast-align
		-Wformat-nonliteral
		-Wformat=2
		-Wsuggest-attribute=format
		-Winvalid-pch
		-Wmissing-declarations
		-Wmissing-format-attribute
		-Wmissing-include-dirs
		-Wredundant-decls
		-Wodr
		-Wfatal-errors
		-fdiagnostics-color=always
		-Wno-narrowing
	)
endif()

target_link_libraries(sorcery_types PRIVATE sorcery_warnings)
target_link_libraries(sorcery_engine PRIVATE sorcery_warnings)
target_link_libraries(sorcery_core PRIVATE sorcery_warnings)
target_link_libraries(sorcery_frontend PRIVATE sorcery_warnings)
target_link_libraries(sorcery_gui PRIVATE sorcery_warnings)
target_link_libraries(sorcery_modules PRIVATE sorcery_warnings)
target_link_libraries(sorcery_resources PRIVATE sorcery_warnings)

# Link Libraries
target_link_libraries(${PROJECT_NAME} PRIVATE
	${SORCERY_SDL2_TARGETS}
	${OPENGL_LIBRARIES}
	Freetype::Freetype
	PkgConfig::LIBAV
	Threads::Threads
	OpenGL::GL
	${SORCERY_GLEW_TARGET}
	${SORCERY_JSONCPP_TARGET}
	dear_imgui
	${SORCERY_SQLITE_TARGET}
	${SORCERY_UUID_LIBRARIES}
	${SORCERY_PLATFORM_LIBRARIES}
	sorcery_core
	sorcery_engine
	sorcery_frontend
	sorcery_gui
	sorcery_modules
	sorcery_resources
	sorcery_types
	sorcery_warnings
)

# Post Build Directories
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E make_directory
	cfg)
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E make_directory
	dat)
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E make_directory
	doc)
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E make_directory
	gfx)
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E make_directory
	sfx)
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E make_directory
	vfx)

# Post Build Copying
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory
	${CMAKE_SOURCE_DIR}/cfg/ $<TARGET_FILE_DIR:${PROJECT_NAME}>/cfg)
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory
	${CMAKE_SOURCE_DIR}/dat/ $<TARGET_FILE_DIR:${PROJECT_NAME}>/dat)
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory
	${CMAKE_SOURCE_DIR}/doc/ $<TARGET_FILE_DIR:${PROJECT_NAME}>/doc)
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory
	${CMAKE_SOURCE_DIR}/gfx/ $<TARGET_FILE_DIR:${PROJECT_NAME}>/gfx)
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory
	${CMAKE_SOURCE_DIR}/sfx/ $<TARGET_FILE_DIR:${PROJECT_NAME}>/sfx)
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory
	${CMAKE_SOURCE_DIR}/vfx/ $<TARGET_FILE_DIR:${PROJECT_NAME}>/vfx)

# Packaging
set(CPACK_PROJECT_NAME sorcery)
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
