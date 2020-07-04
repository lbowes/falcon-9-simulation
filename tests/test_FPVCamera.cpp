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

            THEN("It is positioned at the origin") {
                const chrono::Vector origin = {0, 0, 0};
                REQUIRE(camPos == origin);
            }
        }
        AND_WHEN("The camera is moved forwards") {
            FPVCamera::Input input;
            input.move.forward = true;
            fpvCam.process(input);

            fpvCam.update(1.0);

            THEN("It is positioned on the -Z axis") {
                const chrono::Vector pos = camSys.getActivePos();

                REQUIRE(pos.x() == Approx(0.0).margin(1e-12));
                REQUIRE(pos.y() == Approx(0.0).margin(1e-12));
                REQUIRE(pos.z() < 0.0);
            }
        }
        AND_WHEN("The camera is moved right") {
            FPVCamera::Input input;
            input.move.right = true;
            fpvCam.process(input);

            fpvCam.update(1.0);

            THEN("It is positioned on the +X axis") {
                const chrono::Vector pos = camSys.getActivePos();

                REQUIRE(pos.x() > 0.0);
                REQUIRE(pos.y() == Approx(0.0).margin(1e-12));
                REQUIRE(pos.z() == Approx(0.0).margin(1e-12));
            }
        }
        AND_WHEN("The camera is moved up") {
            FPVCamera::Input input;
            input.move.up = true;
            fpvCam.process(input);

            fpvCam.update(1.0);

            THEN("It is positioned on the +Y axis") {
                const chrono::Vector pos = camSys.getActivePos();

                REQUIRE(pos.x() == Approx(0.0).margin(1e-12));
                REQUIRE(pos.y() > 0.0);
                REQUIRE(pos.z() == Approx(0.0).margin(1e-12));
            }
        }
    }
}

SCENARIO("Rotating an FPVCamera", "[FPVCamera]") {
    GIVEN("An FPVCamera instance registered with the CameraSystem") {
        using namespace F9Sim::Graphics;

        FPVCamera fpvCam;
        CameraSystem& camSys = CameraSystem::getInstance();

        WHEN("The camera is rotated 90 degrees clockwise and moved forward") {
            FPVCamera::Input input;
            input.yawDelta_degs = 90.0f;
            input.move.forward = true;
            fpvCam.process(input);

            fpvCam.update(1.0);

            THEN("It is positioned on the +X") {
                const chrono::Vector pos = camSys.getActivePos();

                REQUIRE(pos.x() > 0.0);
                REQUIRE(pos.y() == Approx(0.0).margin(1e-12));
                REQUIRE(pos.z() == Approx(0.0).margin(1e-12));
            }
        }
        AND_WHEN("The camera is rotated 90 degrees clockwise and moved forward") {
            FPVCamera::Input input;
            input.yawDelta_degs = 90.0f;
            input.move.forward = true;
            fpvCam.process(input);

            fpvCam.update(1.0);

            THEN("It is positioned on the +X") {
                const chrono::Vector pos = camSys.getActivePos();

                REQUIRE(pos.x() > 0.0);
                REQUIRE(pos.y() == Approx(0.0).margin(1e-12));
                REQUIRE(pos.z() == Approx(0.0).margin(1e-12));
            }
        }
    }
}
