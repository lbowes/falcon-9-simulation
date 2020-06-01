#include "../3rd_party/catch.hpp"
#include "Simulation.h"

#include <fstream>


static const char* outputFile = "output.json";


static bool fileExists(const char* file) {
    std::ifstream f(file);
    return f.good();
}


static bool fileEmpty(const char* file) {
    std::ifstream f(file);
    return f.peek() == std::ifstream::traits_type::eof();
}


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
