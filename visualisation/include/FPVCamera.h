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
        double pitchDelta_degs;
        double yawDelta_degs;

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
        static const double zoom;
        static const double adjustSpeed;
    };

    struct Movement {
        static const double minSpeed;
        static const double maxSpeed;
        static const double friction;
    };

    CameraBaseState m_camera;
    glm::dvec3 m_accelVec;
    glm::dvec3 m_velocity;
    double m_accel;
    double m_pitch;
    double m_yaw;

public:
    FPVCamera();
    ~FPVCamera() = default;

    void process(Input input);
    void update(double dt);

private:
    void moveInput(Input::Move move);
    void directionInput(double pitchDelta_degs, double yawDelta_degs);
    void zoomInput();
    void clampPitchYaw();
    void syncLookAtWithPitchYaw();
    void syncPitchYawWithLookAt();
};


} // namespace Graphics
} // namespace F9Sim


#endif // F9SIM_GRAPHICS_FPVCAMERA_H_

