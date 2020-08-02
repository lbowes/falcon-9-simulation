#include "MountedCamera.h"
#include "CameraSystem.h"

#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>


namespace F9Sim {
namespace Graphics {


MountedCamera::MountedCamera(CameraSystem& camSys) {
    camSys.registerCam(m_camera, "mounted");
}


void MountedCamera::setParentTransform(Transform newTransform) {
    // This function should take in enough information about the cube object that the camera is attached to, to
    // update the camera's position and orientation as the object moves. That is it.
    const glm::dquat orientation_corrected = {-newTransform.orientation.w, newTransform.orientation.z, newTransform.orientation.y, newTransform.orientation.x};
    const glm::dmat4 rotation = glm::toMat4(orientation_corrected);
    const glm::dmat4 t = glm::translate(glm::dmat4(), newTransform.position) * glm::toMat4(orientation_corrected);

    //m_camera.position = glm::dmat3(t) * glm::dvec3(0.0, 0.0, 0.0);
    //m_camera.position = glm::dvec3(0.0, -2.0, 4.0);
    m_camera.position = glm::dvec3(0.0, 1.0, 0.0);
    m_camera.up = orientation_corrected * glm::dvec3(0.0, -1.0, 0.0);
    m_camera.lookAt = orientation_corrected * glm::dvec3(0.0, 0.0, -1.0);
}


} // namespace Graphics
} // namespace F9Sim
