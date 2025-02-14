cmake_minimum_required(VERSION 3.30)
project(Sandbox)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../Target)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

if (NOT DISABLE_ASAN)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-omit-frame-pointer")
endif()

add_compile_options(-Wall -Wextra -pedantic)

file(GLOB_RECURSE sandbox_sources "src/**.cpp")
file(GLOB_RECURSE sandbox_headers "src/**.h")

add_executable(Sandbox
        ${sandbox_sources}
        ${sandbox_headers}
)

target_link_libraries(Sandbox PRIVATE
        Engine
)

target_include_directories(Sandbox PRIVATE
        ${CMAKE_SOURCE_DIR}/Engine/
        ${CMAKE_SOURCE_DIR}/Engine/src
)

