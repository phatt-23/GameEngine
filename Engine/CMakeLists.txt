cmake_minimum_required(VERSION 3.30)
project(Engine)

set(CMAKE_CXX_STANDARD 26)

file(GLOB_RECURSE headers "src/**.h")
file(GLOB_RECURSE sources "src/**.cpp")

add_library(Engine STATIC
        ${sources}
        ${headers}
        ./Engine.h
)

target_include_directories(Engine PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/vendor
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Engine
)

target_precompile_headers(Engine PUBLIC
        src/Engine/EnginePCH.h
)

find_package(spdlog CONFIG REQUIRED)

target_link_libraries(Engine PUBLIC
        spdlog::spdlog
)