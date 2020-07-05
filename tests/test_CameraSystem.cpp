#include "../3rd_party/catch.hpp"
#include "CameraSystem.h"

#include <algorithm>
#include <random>


static float randReal(double min, double max) {
    std::random_device device;
    std::mt19937 generator(device());
    std::uniform_real_distribution<double> dist(min, max);

    return dist(generator);
}


static glm::dvec3 randPos() {
    return {
        randReal(-100.0, 100.0),
        randReal(-100.0, 100.0),
        randReal(-100.0, 100.0)};
}


SCENARIO("Cameras are registered and bound to the CameraSystem", "[CameraSystem]") {
    using namespace F9Sim::Graphics;

    CameraSystem camSys;
    CameraBaseState newCamera;
    newCamera.position = randPos();

    GIVEN("A default CameraSystem") {
        WHEN("No cameras are registered") {
            THEN("The default camera is the only one registered") {
                const std::vector<std::string> registeredCams = camSys.getRegisteredCamNames();

                REQUIRE(registeredCams.size() == 1);
                REQUIRE(registeredCams[0] == "default_camera");
            }
            THEN("The default camera is positioned at the origin") {
                const glm::dvec3 pos = camSys.getActivePos();
                const glm::dvec3 origin = {0, 0, 0};

                REQUIRE(pos == origin);
            }
        }
    }
    GIVEN("One camera is registered with the CameraSystem") {
        const std::string name = "name";
        bool registered = camSys.registerCam(newCamera, name);

        WHEN("The name used to register is unique") {
            THEN("The camera is successfully registered") {
                REQUIRE(registered);
            }
            AND_THEN("The camera is listed as being registered") {
                const std::vector<std::string> registeredCams = camSys.getRegisteredCamNames();

                bool found = std::find(registeredCams.begin(), registeredCams.end(), name) != registeredCams.end();
                REQUIRE(found);
            }
        }
        AND_WHEN("A camera is registered with an already-used name") {
            bool registered = camSys.registerCam(newCamera, name);

            THEN("The camera is not successfully registered") {
                REQUIRE(!registered);
            }
            AND_THEN("The camera's name is only shown once in the list of registered cameras") {
                const std::vector<std::string> registeredCams = camSys.getRegisteredCamNames();
                int count = std::count(registeredCams.begin(), registeredCams.end(), name);

                REQUIRE(count == 1);
            }
        }
        AND_WHEN("The user attempts to bind a registered camera") {
            bool bound = camSys.bind(name);

            THEN("The camera is successfully bound") {
                REQUIRE(bound);
            }
            AND_THEN("The position of the camera is the same as the active camera") {
                const glm::dvec3 activeCamPos = camSys.getActivePos();
                REQUIRE(activeCamPos == newCamera.position);
            }
        }
        AND_WHEN("The user attempts to bind an unregistered camera") {
            bool bound = camSys.bind("unregistered_name");

            THEN("The camera is not successfully bound") {
                REQUIRE(!bound);
            }
        }
    }
}
