cmake_minimum_required(VERSION 3.13.0)

set(CMAKE_CXX_STANDARD 17)

project(f9-sim)

LIST(APPEND CMAKE_PREFIX_PATH "${CMAKE_INSTALL_PREFIX}/../Chrono/lib")
find_package(Chrono CONFIG)

if(NOT Chrono_FOUND)
	message("Could not find Chrono or one of its required modules")
	return()
endif()

include_directories(${CHRONO_INCLUDE_DIRS})
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(BUILD_TESTS FALSE CACHE BOOL "Build tests")
if(BUILD_TESTS)
	add_definitions(-DF9SIM_TEST)
	add_subdirectory(tests)
endif()

add_subdirectory(simulation)
add_subdirectory(visualisation)
