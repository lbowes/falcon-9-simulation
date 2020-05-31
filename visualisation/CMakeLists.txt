# ========== Visualisation ========== 
include_directories("include/")
include_directories("../simulation/include")

add_executable(f9-sim-visualisation)

target_sources(f9-sim-visualisation
	PRIVATE
		src/main.cpp
)

# Copy the resources folder to the build tree
set(source "${CMAKE_CURRENT_SOURCE_DIR}/resources")
set(destination "${CMAKE_CURRENT_BINARY_DIR}/resources")

# Remove the old resource directory from the build tree
add_custom_command(
	TARGET f9-sim-visualisation POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E remove_directory
	${destination})

# Copy the resources directory from the source tree into the build tree
add_custom_command(
	TARGET f9-sim-visualisation POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
	${source}
	${destination})

