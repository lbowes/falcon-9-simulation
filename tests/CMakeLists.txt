set(TEST_SRC
	test_main.cpp
	test_Simulation.cpp
	test_FPVCamera.cpp
	test_CameraSystem.cpp
	test_Animation.cpp
)

add_executable(f9sim-tests ${TEST_SRC})

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

target_include_directories(f9sim-tests
	../simulation/include
	../visualisation/include
)

target_link_libraries(f9sim-tests
	f9sim-simulation-lib
	f9sim-visualisation-lib
)