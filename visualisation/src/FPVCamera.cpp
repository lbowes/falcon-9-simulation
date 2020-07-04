#include "FPVCamera.h"
#include "../3rd_party/imgui/imgui.h"
#include "CameraSystem.h"

#include <glm/geometric.hpp>
#include <glm/trigonometric.hpp>


namespace F9Sim {
namespace Graphics {


const float FPVCamera::Sensitivity::zoom = 0.1f;
const float FPVCamera::Sensitivity::adjustSpeed = 200.0f;
const float FPVCamera::Movement::minSpeed = 6.4f;
const float FPVCamera::Movement::maxSpeed = 1000.0f;
const float FPVCamera::Movement::friction = 7.0f;


FPVCamera::Input::Input() {
    pitchDelta_degs = false;
    yawDelta_degs = false;
    move.forward = false;
    move.backwards = false;
    move.left = false;
    move.right = false;
    move.up = false;
    move.down = false;
}


FPVCamera::FPVCamera() :
    m_velocity(glm::dvec3()),
    m_accelVec(glm::dvec3()),
    m_accel(100.0f),
    m_pitch(0.0f),
    m_yaw(0.0f) {

    m_camera.near = 0.1f;
    m_camera.far = 100.0f;
    m_camera.aspectRatio = 1.0f;
    m_camera.verticalFOV = 44.7f;
    m_camera.position = {0.0, 0.0, 0.0};
    m_camera.up = {0, 1, 0};
    m_camera.lookAt = {0, 0, -1};

    syncPitchYawWithLookAt();
    CameraSystem::getInstance().registerCam(m_camera, "first_person_view");
}


void FPVCamera::process(Input input) {
    directionInput(input.pitchDelta_degs, input.yawDelta_degs);
    moveInput(input.move);
    zoomInput();
}


void FPVCamera::update(double dt) {
    m_velocity += m_accelVec * m_accel * dt;
    m_velocity *= 1.0 / (1.0 + (Movement::friction * dt));
    m_camera.position += m_velocity * dt;
}


void FPVCamera::moveInput(Input::Move move) {
    m_camera.lookAt = glm::normalize(m_camera.lookAt);

    glm::dvec3 forward = {m_camera.lookAt.x, 0.0, m_camera.lookAt.z};
    forward = glm::normalize(forward);

    m_accelVec = glm::dvec3();

    if(move.forward)
        m_accelVec += forward;

    if(move.backwards)
        m_accelVec -= forward;

    const glm::dvec3 up = {0, 1, 0};
    const glm::dvec3 right = glm::cross(forward, up);

    if(move.right)
        m_accelVec += right;

    if(move.left)
        m_accelVec -= right;

    if(move.up)
        m_accelVec += up;

    if(move.down)
        m_accelVec -= up;
}


void FPVCamera::directionInput(float pitchDelta_degs, float yawDelta_degs) {
    m_pitch += pitchDelta_degs;
    m_yaw += yawDelta_degs;

    clampPitchYaw();

    syncLookAtWithPitchYaw();
}


void FPVCamera::zoomInput() {
}


void FPVCamera::clampPitchYaw() {
    m_yaw -= 360.0f * floor(m_yaw / 360.0f);
    m_pitch = m_pitch < -89.9f ? -89.9f : (m_pitch > 89.9f ? 89.9f : m_pitch);
}


void FPVCamera::syncLookAtWithPitchYaw() {
    glm::dvec3 result;

    // Since a yaw of 0 corresponds to -Z, we need to offset m_yaw by -90 degrees for trig calculations to work
    float yawOffset = m_yaw - 90.0f;

    result.x = cos(glm::radians(m_pitch)) * cos(glm::radians(yawOffset));
    result.y = sin(glm::radians(m_pitch));
    result.z = cos(glm::radians(m_pitch)) * sin(glm::radians(yawOffset));
    result = glm::normalize(result);

    m_camera.lookAt = result;
}


void FPVCamera::syncPitchYawWithLookAt() {
    m_camera.lookAt = glm::normalize(m_camera.lookAt);

    // Horizontal component of the look-at vector
    const glm::dvec3 horizLookAt = glm::normalize(glm::dvec3(m_camera.lookAt.x, 0.0, m_camera.lookAt.z));

    const glm::dvec3 forward = {0, 0, -1};
    m_yaw = glm::degrees(acos(glm::dot(horizLookAt, forward)) * (horizLookAt.x < 0.0 ? -1 : 1));

    // Vertical component of look-at vector
    const glm::dvec3 up = {0, 1, 0};
    m_pitch = 90.0f - glm::degrees(acos(glm::dot(m_camera.lookAt, up)));
}


} // namespace Graphics
} // namespace F9Sim
