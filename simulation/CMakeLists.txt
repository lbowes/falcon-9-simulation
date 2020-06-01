include_directories("include/")

# ========== Simulation library ==========
add_library(f9sim-simulation-lib)

target_sources(f9sim-simulation-lib
	PRIVATE
		src/Simulation.cpp
)

set_target_properties(f9sim-simulation-lib PROPERTIES 
	COMPILE_FLAGS "${CHRONO_CXX_FLAGS}"
	COMPILE_DEFINITIONS "CHRONO_DATA_DIR=\"${CHRONO_DATA_DIR}\""
	LINK_FLAGS "${CHRONO_LINKER_FLAGS}"
)

target_link_libraries(f9sim-simulation-lib
	${CHRONO_LIBRARIES}
)

# ========== Simulation application ==========
add_executable(f9sim-simulation)

target_sources(f9sim-simulation
	PRIVATE
		src/main.cpp
)

target_link_libraries(f9sim-simulation
	f9sim-simulation-lib
)
