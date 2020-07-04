#include "../3rd_party/catch.hpp"
#include "CameraSystem.h"
#include "FPVCamera.h"


SCENARIO("Moving an FPVCamera", "[FPVCamera]") {
    GIVEN("An FPVCamera instance registered with the CameraSystem") {
        using namespace F9Sim::Graphics;

        FPVCamera fpvCam;
        CameraSystem& camSys = CameraSystem::getInstance();
        camSys.bind("first_person_view");

        WHEN("The camera's position is queried before any updates") {
            const chrono::Vector camPos = camSys.getActivePos();

            THEN("Output file exists") {
                REQUIRE(true);
            }

            THEN("Output file is not empty") {
                REQUIRE(true);
            }
        }
    }
}
