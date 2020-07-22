#ifndef F9SIM_GRAPHICS_MOUNTEDCAMERA_H_
#define F9SIM_GRAPHICS_MOUNTEDCAMERA_H_
#pragma once


#include "CameraBaseState.h"


namespace F9Sim {
namespace Graphics {


class CameraSystem;

class MountedCamera {
private:
    CameraBaseState m_camera;

public:
    MountedCamera(CameraSystem& camSys);
    ~MountedCamera() = default;
};


} // namespace Graphics
} // namespace F9Sim


#endif // F9SIM_GRAPHICS_MOUNTEDCAMERA_H_

