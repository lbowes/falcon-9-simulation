include_directories("include/")
include_directories("../")
include_directories("../simulation/include")
include_directories(${GLFW_INCLUDE_DIRS})

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

# ========== Visualisation library ========== 
add_library(f9sim-visualisation-lib)

set(IMGUI_SRC 
	../3rd_party/imgui/imgui.cpp
	../3rd_party/imgui/imgui_draw.cpp
	../3rd_party/imgui/imgui_demo.cpp
	../3rd_party/imgui/imgui_impl_glfw.cpp
	../3rd_party/imgui/imgui_widgets.cpp
	../3rd_party/imgui/imgui_impl_bgfx.cpp
)

target_sources(f9sim-visualisation-lib
	PRIVATE
		src/Visualisation.cpp
		src/Mesh.cpp
		src/History.cpp
		src/CameraSystem.cpp
		src/StateSnapshot.cpp
		src/FPVCamera.cpp
		src/MountedCamera.cpp
		src/ShaderUtils.cpp
		src/Input.cpp
		src/Scene.cpp
		${IMGUI_SRC}
)

# Link to libraries
find_package(PkgConfig REQUIRED)

# GLFW
pkg_search_module(GLFW REQUIRED glfw3)

# OpenGL
find_package(OpenGL REQUIRED)
if(NOT OPENGL_FOUND)
    message("ERROR: OpenGL not found")
endif(NOT OPENGL_FOUND)
set(GL_LIBRARY GL GLU X11)

# BGFX
target_link_libraries(f9sim-visualisation-lib
	bgfx
	bimg
	bx
	astc-codec
	${GL_LIBRARY}
	${GLFW_STATIC_LIBRARIES}
)

# ========== Visualisation app ==========
add_executable(f9sim-visualisation)

target_sources(f9sim-visualisation
	PRIVATE
		src/main.cpp
)

target_link_libraries(f9sim-visualisation
	f9sim-visualisation-lib
)

# Copy the resources folder to the build tree
set(source "${CMAKE_CURRENT_SOURCE_DIR}/resources")
set(destination "${CMAKE_CURRENT_BINARY_DIR}/resources")

add_custom_target(copy_resources ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory 
	${source} ${destination}
    VERBATIM)
