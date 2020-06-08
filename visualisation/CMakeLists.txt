# ========== Visualisation ========== 
include_directories("include/")
include_directories("../simulation/include")
include_directories(${GLFW_INCLUDE_DIRS})

add_executable(f9sim-visualisation)

target_sources(f9sim-visualisation
	PRIVATE
		src/main.cpp
)

# Link to libraries
find_package(PkgConfig REQUIRED)

# GLFW
pkg_search_module(GLFW REQUIRED glfw3)
target_link_libraries(f9sim-visualisation ${GLFW_STATIC_LIBRARIES})

# OpenGL
find_package(OpenGL REQUIRED)
if(NOT OPENGL_FOUND)
    message("ERROR: OpenGL not found")
endif(NOT OPENGL_FOUND)
set(GL_LIBRARY GL GLU X11)

target_link_libraries(f9sim-visualisation ${GL_LIBRARY})

# BGFX
target_link_libraries(f9sim-visualisation
	bgfx
	bx
	bimg
	astc-codec
)

# Copy the resources folder to the build tree
set(source "${CMAKE_CURRENT_SOURCE_DIR}/resources")
set(destination "${CMAKE_CURRENT_BINARY_DIR}/resources")

add_custom_target(copy_resources ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory 
	${source} ${destination}
    VERBATIM)
