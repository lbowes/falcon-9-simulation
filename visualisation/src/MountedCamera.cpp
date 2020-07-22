#include "MountedCamera.h"
#include "CameraSystem.h"


namespace F9Sim {
namespace Graphics {


MountedCamera::MountedCamera(CameraSystem& camSys) {
    m_camera.position = {0.0, 0.0, 20.0};

    camSys.registerCam(m_camera, "mounted");
}


} // namespace Graphics
} // namespace F9Sim
