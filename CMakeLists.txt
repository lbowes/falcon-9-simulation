cmake_minimum_required(VERSION 3.13.0)

project(f9-sim)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(BUILD_TESTS FALSE CACHE BOOL "Build tests")
if(BUILD_TESTS)
	add_definitions(-DAIL_TEST)
	add_subdirectory(tests)
endif()

add_subdirectory(simulation)
add_subdirectory(visualisation)
