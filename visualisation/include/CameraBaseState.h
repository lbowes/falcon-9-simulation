#ifndef F9SIM_GRAPHICS_CAMERABASESTATE_H_
#define F9SIM_GRAPHICS_CAMERABASESTATE_H_
#pragma once


#include <chrono/core/ChVector.h>


namespace F9Sim {
namespace Graphics {


struct CameraBaseState {
    float aspectRatio;
    float nearValue;
    float farValue;
    float verticalFOV;
    chrono::ChVector<> position;
    chrono::ChVector<> up;
    chrono::ChVector<> lookAt;
};


} // namespace Graphics
} // namespace F9Sim


#endif // F9SIM_GRAPHICS_CAMERABASESTATE_H_

