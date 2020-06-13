#ifndef F9SIM_GRAPHICS_FPVCAMERA_H_
#define F9SIM_GRAPHICS_FPVCAMERA_H_
#pragma once


#include "CameraBaseState.h"


namespace F9Sim {
namespace Graphics {


class FPVCamera {
public:
    CameraBaseState m_camera;

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

    chrono::ChVector<> m_velocity;
    float m_movementSpeed;
    float m_pitch;
    float m_yaw;

public:
    FPVCamera();
    ~FPVCamera() = default;

    void handleInput(double dt);
    void update(double dt);

private:
    void handleMovementInput(double dt);
    void handleDirectionInput();
    void handleZoomInput(double dt);
    void clampPitchYaw();
    void syncLookAtWithPitchYaw();
    void syncPitchYawWithLookAt();
};


} // namespace Graphics
} // namespace F9Sim


#endif // F9SIM_GRAPHICS_FPVCAMERA_H_

