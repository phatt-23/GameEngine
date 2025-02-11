cmake_minimum_required(VERSION 3.30)
project(Engine)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE engine_headers "src/Engine/**.h" "src/Platform/**.h")
file(GLOB_RECURSE engine_sources "src/Engine/**.cpp" "src/Platform/**.cpp")

add_library(Engine STATIC
        ${engine_headers}
        ${engine_sources}
        Engine.h
)

target_include_directories(Engine PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Engine
        ${CMAKE_CURRENT_SOURCE_DIR}/vendor
        ${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui
)


target_precompile_headers(Engine PUBLIC src/Engine/EnginePCH.h)

target_compile_definitions(Engine
        PUBLIC ENGINE_ENABLE_ASSERTS
        PUBLIC ENGINE_FUNCTION_CURRYING_USE_LAMBDA
        PUBLIC GLFW_INCLUDE_NONE
)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GLFW3 REQUIRED glfw3)
target_link_libraries(Engine PUBLIC glfw)

find_package(glm REQUIRED)
target_link_libraries(Engine PUBLIC glm::glm)

add_subdirectory(vendor/glad)
target_link_libraries(Engine PUBLIC glad)

find_package(spdlog CONFIG REQUIRED)
target_link_libraries(Engine PUBLIC spdlog::spdlog)

add_subdirectory(vendor/imgui)
target_link_libraries(Engine PUBLIC ImGui)

# Optional: If you want CLion to recognize the project independently
add_library(Engine::Engine ALIAS Engine)
