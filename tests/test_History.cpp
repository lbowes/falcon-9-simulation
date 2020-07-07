#include "../3rd_party/catch.hpp"
#include "../3rd_party/json.hpp"
#include "History.h"

#include <random>


static double randTime(double min, double max) {
    std::random_device device;
    std::mt19937 generator(device());
    std::uniform_real_distribution<double> dist(min, max);

    return dist(generator);
}


SCENARIO("Using the simulation state history", "[History]") {
    using namespace F9Sim::Graphics;

    GIVEN("An initialised simulation history") {
        nlohmann::json simData;
        History history(simData);

        WHEN("The user requests the state of the simulation at some point in time") {
            double time = randTime(0.0, 10.0);

            StateSnapshot someSnapshot = history.stateAt(time);
        }
    }
}
