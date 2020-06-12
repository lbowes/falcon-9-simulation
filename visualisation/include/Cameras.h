#ifndef F9SIM_GRAPHICS_CAMERASYSTEM_H_
#define F9SIM_GRAPHICS_CAMERASYSTEM_H_
#pragma once


#include <chrono/core/ChVector.h>


namespace F9Sim {
namespace Graphics {


class CameraBaseState;


void Cameras_init();
chrono::Vector Cameras_getActivePos();
void Cameras_setViewTransform(float aspectRatio, float dt);


} // namespace Graphics
} // namespace F9Sim


#endif // F9SIM_GRAPHICS_CAMERASYSTEM_H_

