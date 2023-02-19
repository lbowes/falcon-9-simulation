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
#if 0
    const glm::dquat orientation_corrected = {-newTransform.orientation.w, newTransform.orientation.z, newTransform.orientation.y, newTransform.orientation.x};
    const glm::dmat4 rotation = glm::toMat4(orientation_corrected);
    const glm::dmat4 t = glm::translate(glm::toMat4(orientation_corrected), newTransform.position);

    m_camera.position = glm::dvec3(t * glm::dvec4(0.0, 1.5, 0.0, 1.0));
    m_camera.up = orientation_corrected * glm::dvec3(0.0, 1.0, 0.0);
    m_camera.lookAt = orientation_corrected * glm::dvec3(0.0, 0.0, -1.0);
#else
    const glm::dquat orientation_corrected = {-newTransform.orientation.w, newTransform.orientation.x, newTransform.orientation.y, newTransform.orientation.x};
    const glm::dmat4 rotation = glm::toMat4(orientation_corrected);
    const glm::dmat4 t = glm::translate(glm::dmat4(1.0f), newTransform.position) * glm::toMat4(newTransform.orientation);

    m_camera.position = glm::dvec3(t * glm::dvec4(0, 0.5, 0, 1));
    printf("m_camera.position: %f, %f, %f\n", m_camera.position.x, m_camera.position.y, m_camera.position.z);
    //m_camera.lookAt = orientation_corrected * glm::dvec3(0, 0, -1);
    //m_camera.up = orientation_corrected * glm::dvec3(0, 1, 0);
#endif
}


} // namespace Graphics
} // namespace F9Sim
