#include "../3rd_party/catch.hpp"
#include "CameraSystem.h"
#include "FPVCamera.h"


static const double errorMargin = 1e-12;


SCENARIO("Moving an FPVCamera", "[FPVCamera]") {
    GIVEN("An FPVCamera instance registered with the CameraSystem") {
        using namespace F9Sim::Graphics;

        FPVCamera fpvCam;
        CameraSystem& camSys = CameraSystem::getInstance();
        camSys.bind("first_person_view");

        WHEN("The camera's position is queried before any movement") {
            const glm::dvec3 camPos = camSys.getActivePos();

            THEN("It is positioned at the origin") {
                const glm::dvec3 origin = {0, 0, 0};
                REQUIRE(camPos == origin);
            }
        }
        AND_WHEN("The camera is moved forwards") {
            FPVCamera::Input input;
            input.move.forward = true;
            fpvCam.process(input);

            fpvCam.update(1.0);

            THEN("It is positioned on the -Z axis") {
                const glm::dvec3 pos = camSys.getActivePos();

                REQUIRE(pos.x == Approx(0.0).margin(errorMargin));
                REQUIRE(pos.y == Approx(0.0).margin(errorMargin));
                REQUIRE(pos.z < 0.0);
            }
        }
        AND_WHEN("The camera is moved right") {
            FPVCamera::Input input;
            input.move.right = true;
            fpvCam.process(input);

            fpvCam.update(1.0);

            THEN("It is positioned on the +X axis") {
                const glm::dvec3 pos = camSys.getActivePos();

                REQUIRE(pos.x > 0.0);
                REQUIRE(pos.y == Approx(0.0).margin(errorMargin));
                REQUIRE(pos.z == Approx(0.0).margin(errorMargin));
            }
        }
        AND_WHEN("The camera is moved up") {
            FPVCamera::Input input;
            input.move.up = true;
            fpvCam.process(input);

            fpvCam.update(1.0);

            THEN("It is positioned on the +Y axis") {
                const glm::dvec3 pos = camSys.getActivePos();

                REQUIRE(pos.x == Approx(0.0).margin(errorMargin));
                REQUIRE(pos.y > 0.0);
                REQUIRE(pos.z == Approx(0.0).margin(errorMargin));
            }
        }
        AND_WHEN("The camera is moved backwards") {
            FPVCamera::Input input;
            input.move.backwards = true;
            fpvCam.process(input);

            fpvCam.update(1.0);

            THEN("It is positioned on the +Z axis") {
                const glm::dvec3 pos = camSys.getActivePos();

                REQUIRE(pos.x == Approx(0.0).margin(errorMargin));
                REQUIRE(pos.y == Approx(0.0).margin(errorMargin));
                REQUIRE(pos.z > 0.0);
            }
        }
        AND_WHEN("The camera is moved left") {
            FPVCamera::Input input;
            input.move.left = true;
            fpvCam.process(input);

            fpvCam.update(1.0);

            THEN("It is positioned on the -X axis") {
                const glm::dvec3 pos = camSys.getActivePos();

                REQUIRE(pos.x < 0.0);
                REQUIRE(pos.y == Approx(0.0).margin(errorMargin));
                REQUIRE(pos.z == Approx(0.0).margin(errorMargin));
            }
        }
        AND_WHEN("The camera is moved down") {
            FPVCamera::Input input;
            input.move.down = true;
            fpvCam.process(input);

            fpvCam.update(1.0);

            THEN("It is positioned on the -Y axis") {
                const glm::dvec3 pos = camSys.getActivePos();

                REQUIRE(pos.x == Approx(0.0).margin(errorMargin));
                REQUIRE(pos.y < 0.0);
                REQUIRE(pos.z == Approx(0.0).margin(errorMargin));
            }
        }
    }
}

SCENARIO("Rotating an FPVCamera", "[FPVCamera]") {
    using namespace F9Sim::Graphics;

    GIVEN("An FPVCamera instance registered with the CameraSystem") {
        FPVCamera fpvCam;
        CameraSystem& camSys = CameraSystem::getInstance();

        WHEN("The camera is rotated 90 degrees clockwise and moved forward") {
            FPVCamera::Input input;
            input.yawDelta_degs = 90.0f;
            input.move.forward = true;
            fpvCam.process(input);

            fpvCam.update(1.0);

            THEN("It is positioned along +X") {
                const glm::dvec3 pos = camSys.getActivePos();

                REQUIRE(pos.x > 0.0);
                REQUIRE(pos.y == Approx(0.0).margin(errorMargin));
                REQUIRE(pos.z == Approx(0.0).margin(errorMargin));
            }
        }
        AND_WHEN("The camera is rotated 90 degrees anticlockwise and moved forward") {
            FPVCamera::Input input;
            input.yawDelta_degs = -90.0f;
            input.move.forward = true;
            fpvCam.process(input);

            fpvCam.update(1.0);

            THEN("It is positioned along -X") {
                const glm::dvec3 pos = camSys.getActivePos();

                REQUIRE(pos.x < 0.0);
                REQUIRE(pos.y == Approx(0.0).margin(errorMargin));
                REQUIRE(pos.z == Approx(0.0).margin(errorMargin));
            }
        }
        AND_WHEN("The camera is rotated 270 degrees clockwise and moved forward") {
            FPVCamera::Input input;
            input.yawDelta_degs = 270.0f;
            input.move.forward = true;
            fpvCam.process(input);

            fpvCam.update(1.0);

            THEN("It is still positioned along the -X") {
                const glm::dvec3 pos = camSys.getActivePos();

                REQUIRE(pos.x < 0.0);
                REQUIRE(pos.y == Approx(0.0).margin(errorMargin));
                REQUIRE(pos.z == Approx(0.0).margin(errorMargin));
            }
        }
        AND_WHEN("The camera is rotated 60 degrees clockwise and moved forward") {
            FPVCamera::Input input;
            input.yawDelta_degs = 60.0f;
            input.move.forward = true;
            fpvCam.process(input);

            fpvCam.update(1.0);

            THEN("It is closer to the +X axis than the -Z axis") {
                const glm::dvec3 pos = camSys.getActivePos();

                REQUIRE(pos.x > abs(pos.z));
            }
        }
        AND_WHEN("The camera is rotated 30 degrees clockwise and moved backward") {
            FPVCamera::Input input;
            input.yawDelta_degs = 30.0f;
            input.move.backwards = true;
            fpvCam.process(input);

            fpvCam.update(1.0);

            THEN("It is closer to the +X axis than the -Z axis") {
                const glm::dvec3 pos = camSys.getActivePos();

                REQUIRE(pos.z > abs(pos.x));
            }
        }
        AND_WHEN("The camera is rotated 345 and then 105 degrees clockwise, causing yaw to wrap around") {
            // Rotate the camera to 345 degrees yaw
            FPVCamera::Input input;
            input.yawDelta_degs = 345.0f;
            fpvCam.process(input);

            // Rotate another 105 degrees clockwise and move forward
            input.yawDelta_degs = 105.0f;
            input.move.forward = true;
            fpvCam.process(input);

            fpvCam.update(1.0);

            THEN("It is positioned along +X") {
                const glm::dvec3 pos = camSys.getActivePos();

                REQUIRE(pos.x > 0.0);
                REQUIRE(pos.y == Approx(0.0).margin(errorMargin));
                REQUIRE(pos.z == Approx(0.0).margin(errorMargin));
            }
        }
        AND_WHEN("The camera is rotated 15 degrees clockwise and then 105 degrees anticlockwise, causing yaw to wrap around") {
            // Rotate the camera 15 degrees clockwise
            FPVCamera::Input input;
            input.yawDelta_degs = 15.0f;
            fpvCam.process(input);

            // Rotate another 105 degrees anticlockwise
            input.yawDelta_degs = -105.0f;
            input.move.forward = true;
            fpvCam.process(input);

            fpvCam.update(1.0);

            THEN("It is positioned along -X") {
                const glm::dvec3 pos = camSys.getActivePos();

                REQUIRE(pos.x < 0.0);
                REQUIRE(pos.y == Approx(0.0).margin(errorMargin));
                REQUIRE(pos.z == Approx(0.0).margin(errorMargin));
            }
        }
    }
}
