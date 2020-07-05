#include "../3rd_party/catch.hpp"
#include "CameraSystem.h"


SCENARIO("The CameraSystem is used", "[CameraSystem]") {
    GIVEN("Access to the CameraSystem, and some CameraBaseState") {
        using namespace F9Sim::Graphics;

        CameraSystem& camSys = CameraSystem::getInstance();
        CameraBaseState camera;

        WHEN("No cameras are registered") {
            THEN("The default camera is the only one registered") {
                const std::vector<std::string> registeredCams = camSys.getRegisteredCamNames();

                REQUIRE(registeredCams.size() == 1);
                REQUIRE(registeredCams[0] == "default_camera");
            }
            THEN("The default camera is positioned at the origin") {
                REQUIRE(true);
            }
        }
        AND_WHEN("A new camera is registered with a new name") {
            THEN("The camera is successfully registered") {
                REQUIRE(true);
            }
            AND_THEN("The camera is listed as being registered") {
                REQUIRE(true);
            }
        }
        AND_WHEN("A camera is registered with an already-registered name") {
            THEN("The camera is not successfully registered") {
                REQUIRE(true);
            }
            AND_THEN("The camera is not listed as being registered") {
                REQUIRE(true);
            }
        }
        AND_WHEN("The user attempts to bind a registered camera") {
            THEN("The camera is successfully bound") {
                REQUIRE(true);
            }
            AND_THEN("The position of the camera is the same as the active camera") {
                REQUIRE(true);
            }
        }
        AND_WHEN("The user attempts to bind an unregistered camera") {
            THEN("The camera is not successfully bound") {
                REQUIRE(true);
            }
        }
    }
}
