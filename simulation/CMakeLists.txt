include_directories("include/")

# ========== Simulation library ==========
add_library(f9-sim-simulation-lib)

target_sources(f9-sim-simulation-lib
	PRIVATE
		src/Simulation.cpp
)

LIST(APPEND CMAKE_PREFIX_PATH "${CMAKE_INSTALL_PREFIX}/../Chrono/lib")
find_package(Chrono CONFIG)

if(NOT Chrono_FOUND)
  message("Could not find Chrono or one of its required modules")
  return()
endif()

include_directories(${CHRONO_INCLUDE_DIRS})

set_target_properties(f9-sim-simulation-lib PROPERTIES 
	COMPILE_FLAGS "${CHRONO_CXX_FLAGS}"
	COMPILE_DEFINITIONS "CHRONO_DATA_DIR=\"${CHRONO_DATA_DIR}\""
	LINK_FLAGS "${CHRONO_LINKER_FLAGS}"
)

target_link_libraries(f9-sim-simulation-lib
	${CHRONO_LIBRARIES}
)

# ========== Simulation application ==========
add_executable(f9-sim-simulation)

target_sources(f9-sim-simulation
	PRIVATE
		src/main.cpp
)

target_link_libraries(f9-sim-simulation
	f9-sim-simulation-lib
)
