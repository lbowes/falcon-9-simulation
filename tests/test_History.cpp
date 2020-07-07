#include "../3rd_party/catch.hpp"
#include "../3rd_party/json.hpp"
#include "History.h"


SCENARIO("Using the simulation state history", "[History]") {
    using namespace F9Sim::Graphics;

    GIVEN("An uninitialised simulation History") {
        History history;

        WHEN("There's some simulation output data") {
            nlohmann::json simData;

            THEN("This can be loaded into the history") {
                history.loadWith(simData);
            }
        }
    }
    GIVEN("An initialised simulation history") {
        WHEN("The user requests the state of the simulation at some point in time") {
            double time = randTime(0.0, 10.0);

            StateSnapshot someSnapshot = history.stateAt(time);
        }
    }
}
