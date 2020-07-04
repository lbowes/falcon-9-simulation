#include "../3rd_party/catch.hpp"
#include "CameraSystem.h"
#include "FPVCamera.h"


SCENARIO("Running a simulation", "[Simulation]") {
    GIVEN("A simulation instance") {
        Simulation sim;

        WHEN("The simulation is run") {
            sim.run();

            THEN("Output file exists") {
                REQUIRE(fileExists(outputFile));
            }

            THEN("Output file is not empty") {
                REQUIRE(!fileEmpty(outputFile));
            }
        }
    }
}
