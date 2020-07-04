#ifndef F9SIM_GRAPHICS_FPVCAMERA_H_
#define F9SIM_GRAPHICS_FPVCAMERA_H_
#pragma once


#include "CameraBaseState.h"

#include <glm/vec3.hpp>


namespace F9Sim {
namespace Graphics {


class FPVCamera {
public:
    struct Input {
        float pitchDelta_degs;
        float yawDelta_degs;

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
        static const float adjustSpeed;
    };

    struct Movement {
        static const float minSpeed;
        static const float maxSpeed;
        static const float friction;
    };

    CameraBaseState m_camera;
    glm::dvec3 m_accelVec;
    glm::dvec3 m_velocity;
    double m_accel;
    float m_pitch;
    float m_yaw;

public:
    FPVCamera();
    ~FPVCamera() = default;

    void process(Input input);
    void update(double dt);

private:
    void moveInput(Input::Move move);
    void directionInput(float pitchDelta_degs, float yawDelta_degs);
    void zoomInput();
    void clampPitchYaw();
    void syncLookAtWithPitchYaw();
    void syncPitchYawWithLookAt();
};


} // namespace Graphics
} // namespace F9Sim


#endif // F9SIM_GRAPHICS_FPVCAMERA_H_

