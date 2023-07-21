cmake_minimum_required(VERSION 3.16)
project(planes)

find_package(raylib 3.0 REQUIRED) # Requires at least version 3.0

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -mavx")

add_subdirectory(src)
add_subdirectory(test)

target_link_libraries(${PROJECT_NAME} raylib)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "") # works
set(CMAKE_EXPORT_COMPILE_COMMANDS ON )

# Checks if OSX and links appropriate frameworks (only required on MacOS)
if (APPLE)
    target_link_libraries(${PROJECT_NAME} "-framework IOKit")
    target_link_libraries(${PROJECT_NAME} "-framework Cocoa")
    target_link_libraries(${PROJECT_NAME} "-framework OpenGL")
endif()
