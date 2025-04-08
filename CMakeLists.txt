cmake_minimum_required(VERSION 3.28)

project(GAME_EDITOR)

set(CMAKE_CXX_STANDARD 23)

option(BUILD_DEMO_APP "Build demo app" ON)

# set(CMAKE_VERBOSE_MAKEFILE ON)
find_package(Vulkan REQUIRED COMPONENTS shaderc_combined)

file(GLOB_RECURSE EditorFiles
    "src/Editor/*.h"
    "src/Editor/*.cpp"
    "src/external/imgui/backends/imgui_impl_vulkan.cpp"
    "src/external/imgui/backends/imgui_impl_glfw.cpp"
    "src/external/imgui/imgui_draw.cpp"
    "src/external/imgui/imgui_tables.cpp"
    "src/external/imgui/imgui_widgets.cpp"
    "src/external/imgui/imgui.cpp"
    "src/external/imgui/imgui_demo.cpp"
)

add_definitions("-DUSING_IMGUI")

if(BUILD_DEMO_APP AND NOT DEFINED USER_SCRIPTS_SOURCE_PATH)
    message("-- BUILD_DEMO_APP defined, adding scripts source path definition")
    set(USER_SCRIPTS_SOURCE_PATH "\"${PROJECT_SOURCE_DIR}/dependencies/GraphicsEngine/src/Demo/Scripts.h\"" CACHE STRING "Path to user scripts definition")
endif()

if(NOT DEFINED USER_COMPONENTS_SOURCE_PATH)
    set(USER_COMPONENTS_SOURCE_PATH "\"${PROJECT_SOURCE_DIR}/src/Editor/Deserialization/EditorComponents.h\"" CACHE STRING "Path to user components definition")
endif()

set(BUILD_DEMO_APPS OFF CACHE BOOL "Build demo apps")
set(DELIVER_RESOURCES OFF CACHE BOOL "Copy resources to binary directory")
add_subdirectory(dependencies/GraphicsEngine)
target_include_directories(Engine PUBLIC "src/external/imgui/")
target_include_directories(Engine PUBLIC "src/Editor/")
target_include_directories(Engine PUBLIC "dependencies/GraphicsEngine/src/")

aux_source_directory(src/external EXTERNAL)

add_compile_options(/Zc:preprocessor)
add_library(Editor STATIC ${EditorFiles})

target_include_directories(Editor PUBLIC "src/Editor/")
target_include_directories(Editor PUBLIC "src/Demo/")
target_include_directories(Editor PUBLIC "src/external/imgui/")
target_include_directories(Editor PUBLIC "dependencies/GraphicsEngine/src/")
target_link_libraries(Editor PUBLIC Engine)

string(LENGTH "${PROJECT_SOURCE_DIR}/src/" SOURCE_PATH_SIZE)
add_compile_definitions(SOURCE_PATH_SIZE=${SOURCE_PATH_SIZE})

if(DEFINED SCRIPT_PUBLICATION_PATH)
    message("-- SCRIPT_PUBLICATION_PATH defined, adding definition")
    add_compile_definitions(SCRIPT_PUBLICATION_SOURCE=${SCRIPT_PUBLICATION_PATH})
else()
    add_compile_definitions(SCRIPT_PUBLICATION_SOURCE="ScriptPublications.h")
endif()

if(DEFINED RESOURCE_PATH)
    add_compile_definitions(RESOURCE_PATH="${RESOURCE_PATH}")
else()
    add_compile_definitions(RESOURCE_PATH="${PROJECT_SOURCE_DIR}/res/")
endif()

add_executable(EditorDemo "src/main.cpp")

target_link_libraries(EditorDemo PUBLIC Editor)

# To store profiles
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/Release/profiles ${CMAKE_BINARY_DIR}/Debug/profiles)

message("-- Build type for GameEditor: ${CMAKE_BUILD_TYPE}")

# To keep assets in the same place as the binaries
message("-- Delivering editor resources")

if(CMAKE_BUILD_TYPE MATCHES Debug)
    add_custom_command(TARGET EditorDemo POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/res ${CMAKE_BINARY_DIR}/Debug/res)
    add_custom_command(TARGET Editor POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/res ${CMAKE_BINARY_DIR}/Debug/res)
else()
    add_custom_command(TARGET EditorDemo POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/res ${CMAKE_BINARY_DIR}/Release/res)
    add_custom_command(TARGET Editor POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/res ${CMAKE_BINARY_DIR}/Release/res)
endif()
