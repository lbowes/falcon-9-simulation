add_executable(f9sim-tests "")

include_directories("../simulation/include")
include_directories("../visualisation/include")

target_link_libraries(f9sim-tests
	f9sim-simulation-lib
	f9sim-visualisation-lib
)

target_sources(f9sim-tests
	PRIVATE
		test_main.cpp
		test_Simulation.cpp
		test_FPVCamera.cpp
		test_CameraSystem.cpp
)
