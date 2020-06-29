#ifndef F9SIM_GRAPHICS_FPVCAMERA_H_
#define F9SIM_GRAPHICS_FPVCAMERA_H_
#pragma once


#include "CameraBaseState.h"

#include <glm/vec2.hpp>


namespace F9Sim {
namespace Graphics {


// todo: Fix camera jump when starting app with mouse over window


class FPVCamera {
public:
    struct Input {
        glm::ivec2 mouseDelta;

        struct Move {
            bool forward;
            bool backwards;
            bool left;
            bool right;
            bool up;
            bool down;
        } move;

        Input();
    };

private:
    struct Sensitivity {
        static const float zoom;
        static const float lookAround;
        static const float adjustSpeed;
    };

    struct Movement {
        static const float minSpeed;
        static const float maxSpeed;
        static const float friction;
    };

    CameraBaseState m_camera;
    chrono::ChVector<> m_velocity;
    float m_movementSpeed;
    float m_pitch;
    float m_yaw;

public:
    FPVCamera();
    ~FPVCamera() = default;

    void process(Input input, double dt);
    void update(double dt);

private:
    void moveInput(Input::Move move, double dt);
    void directionInput(glm::ivec2 mouseDelta);
    void zoomInput(double dt);
    void clampPitchYaw();
    void syncLookAtWithPitchYaw();
    void syncPitchYawWithLookAt();
};


} // namespace Graphics
} // namespace F9Sim


#endif // F9SIM_GRAPHICS_FPVCAMERA_H_

