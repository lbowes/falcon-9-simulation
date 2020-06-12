#ifndef F9SIM_GRAPHICS_CAMERABASESTATE_H_
#define F9SIM_GRAPHICS_CAMERABASESTATE_H_
#pragma once


#include <chrono/core/ChVector.h>


namespace F9Sim {
namespace Graphics {


struct CameraBaseState {
    float aspectRatio;
    float near;
    float far;
    float verticalFOV;
    chrono::Vector position;
    chrono::Vector up;
    chrono::Vector lookAt;
};


} // namespace Graphics
} // namespace F9Sim


#endif // F9SIM_GRAPHICS_CAMERABASESTATE_H_

