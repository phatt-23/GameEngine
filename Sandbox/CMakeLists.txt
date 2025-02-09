cmake_minimum_required(VERSION 3.30)
project(Sandbox)

set(CMAKE_CXX_STANDARD 26)

add_executable(Sandbox)

file(GLOB_RECURSE sources "src/**.cpp")
file(GLOB_RECURSE headers "src/**.h")

target_sources(Sandbox PRIVATE
        ${sources}
        ${headers}
)

target_link_libraries(Sandbox PRIVATE
        Engine
)

target_include_directories(Sandbox PRIVATE
        ${CMAKE_SOURCE_DIR}/Engine
)



