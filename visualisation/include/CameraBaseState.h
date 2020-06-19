#ifndef F9SIM_GRAPHICS_CAMERABASESTATE_H_
#define F9SIM_GRAPHICS_CAMERABASESTATE_H_
#pragma once


#include <chrono/core/ChVector.h>


namespace F9Sim {
namespace Graphics {


struct CameraBaseState {
    // note: This isn't used yet. The aspect ratio of the screen is used at all times but in future cameras could have
    // more unique profiles and different aspect ratios.
    float aspectRatio;

    float near;
    float far;
    float verticalFOV;
    chrono::Vector position;
    chrono::Vector up;
    chrono::Vector lookAt;


    CameraBaseState() {
        // Default state
        aspectRatio = 16.0f / 9.0f;
        near = 0.1f;
        far = 100.0f;
        verticalFOV = 60.0f;
        position = {0.0f, 0.0f, 0.0f};
        up = {0.0f, 1.0f, 0.0f};
        lookAt = {0.0f, 0.0f, -1.0f};
    }


    CameraBaseState(const CameraBaseState& other) {
        aspectRatio = other.aspectRatio;
        near = other.near;
        far = other.far;
        verticalFOV = other.verticalFOV;
        position = other.position;
        up = other.up;
        lookAt = other.lookAt;
    }
};


} // namespace Graphics
} // namespace F9Sim


#endif // F9SIM_GRAPHICS_CAMERABASESTATE_H_

