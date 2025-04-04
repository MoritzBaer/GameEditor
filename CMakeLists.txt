cmake_minimum_required(VERSION 3.28)

project(GAME_EDITOR)

set(CMAKE_CXX_STANDARD 23)

# set(CMAKE_VERBOSE_MAKEFILE ON)
find_package(Vulkan REQUIRED COMPONENTS shaderc_combined)

include_directories("src/Editor")
include_directories("src/Demo")
include_directories("dependencies/GraphicsEngine/src/")

# Repeat includes from GraphicsEngine
include_directories("dependencies/GraphicsEngine/src/Engine") # Only needed for includes between Engine files
include_directories(${Vulkan_INCLUDE_DIR})
include_directories("src/external/imgui")
include_directories("dependencies/GraphicsEngine/src/external/json-parsing")
include_directories("dependencies/GraphicsEngine/src/external/stb")
include_directories("dependencies/GraphicsEngine/dependencies/glfw-3.3.9.bin.WIN64/include")
include_directories("dependencies/GraphicsEngine/dependencies/VMA")
include_directories("dependencies/GraphicsEngine/dependencies/GLM/")

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

aux_source_directory(src/external EXTERNAL)

add_compile_options(/Zc:preprocessor)
add_definitions("-DUSING_IMGUI")
add_library(Editor STATIC ${EditorFiles})

string(LENGTH "${CMAKE_SOURCE_DIR}/src/" SOURCE_PATH_SIZE)
add_compile_definitions(SOURCE_PATH_SIZE=${SOURCE_PATH_SIZE})

if(NOT DEFINED USER_COMPONENTS_SOURCE_PATH)
    set(USER_COMPONENTS_SOURCE_PATH "\"${CMAKE_SOURCE_DIR}/src/Editor/Deserialization/EditorComponents.h\"" CACHE STRING "Path to user components definition")
endif()

if(DEFINED SCRIPT_PUBLICATION_PATH)
    message("SCRIPT_PUBLICATION_PATH defined, adding definition")
    add_compile_definitions(SCRIPT_PUBLICATION_SOURCE=${SCRIPT_PUBLICATION_PATH})
else()
    add_compile_definitions(SCRIPT_PUBLICATION_SOURCE="ScriptPublications.h")
endif()

if(DEFINED RESOURCE_PATH)
    add_compile_definitions(RESOURCE_PATH="${RESOURCE_PATH}")
else()
    add_compile_definitions(RESOURCE_PATH="${CMAKE_SOURCE_DIR}/res/")
endif()

add_executable(EditorDemo "src/main.cpp")

set(BUILD_DEMO_APPS OFF CACHE BOOL "Build demo apps")
add_subdirectory(dependencies/GraphicsEngine)

target_link_libraries(EditorDemo Editor)
target_link_libraries(EditorDemo Engine)
target_link_libraries(EditorDemo "${CMAKE_SOURCE_DIR}/dependencies/GraphicsEngine/dependencies/glfw-3.3.9.bin.WIN64/lib-vc2022/glfw3_mt.lib")
target_link_libraries(EditorDemo Vulkan::Vulkan)
target_link_libraries(EditorDemo Vulkan::shaderc_combined)

# To store profiles
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/Release/profiles ${CMAKE_BINARY_DIR}/Debug/profiles)

# To keep assets in the same place as the binaries
if(CMAKE_BUILD_TYPE MATCHES Debug)
    add_custom_command(TARGET EditorDemo POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/res ${CMAKE_BINARY_DIR}/Debug/res)
else()
    add_custom_command(TARGET EditorDemo POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/res ${CMAKE_BINARY_DIR}/Release/res)
endif()
