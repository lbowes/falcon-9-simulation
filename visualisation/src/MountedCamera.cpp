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
    m_camera.position = glm::dvec3(0.0, 0.5, 0.0) * newTransform;

    const glm::dquat t = {
        -newTransform.orientation.w,
        newTransform.orientation.x,
        newTransform.orientation.y,
        newTransform.orientation.z,
    };

    m_camera.lookAt = t * glm::dvec3(0.0, 0.0, -1.0);
}


} // namespace Graphics
} // namespace F9Sim
