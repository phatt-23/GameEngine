cmake_minimum_required(VERSION 3.30)

project(Workspace)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(Engine)
add_subdirectory(Sandbox)

# cmake --build . --target run
add_custom_target(run
    COMMAND ${CMAKE_COMMAND} -E chdir ${CMAKE_SOURCE_DIR} $<TARGET_FILE:Sandbox>
    DEPENDS Sandbox 
)

# ctest
add_test(NAME MyTest
    COMMAND Sandbox 
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

