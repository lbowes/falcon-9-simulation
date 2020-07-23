#include "MountedCamera.h"
#include "CameraSystem.h"


namespace F9Sim {
namespace Graphics {


MountedCamera::MountedCamera(CameraSystem& camSys) {
    m_camera.position = {0.0, 0.0, 20.0};

    camSys.registerCam(m_camera, "mounted");
}


void MountedCamera::setParentTransform(Transform newTransform) {
    // todo: use the parent transform passed in to update the internal `m_camera` state variables, position, lookAt, up,
    // etc such that the camera moves with the parent's reference frame.

    // temp
    m_camera.position = newTransform.position + glm::dvec3(0.0, 1.0, 0.0);
    m_camera.lookAt = glm::dvec3(0.0, 0.0, -1.0) * newTransform.orientation;
}


} // namespace Graphics
} // namespace F9Sim
