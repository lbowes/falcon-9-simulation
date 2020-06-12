#include "FPVCamera.h"
#include "../3rd_party/imgui/imgui.h"
#include "Cameras.h"
#include "Input.h"

#include <GLFW/glfw3.h>
#include <chrono/core/ChMathematics.h>
#include <chrono/core/ChVector.h>
#include <glm/vec2.hpp>


namespace F9Sim {
namespace Graphics {


const float FPVCamera::Sensitivity::zoom = 0.1f;
const float FPVCamera::Sensitivity::lookAround = 0.05f;
const float FPVCamera::Sensitivity::adjustSpeed = 200.0f;
const float FPVCamera::Movement::minSpeed = 6.4f;
const float FPVCamera::Movement::maxSpeed = 1000.0f;
const float FPVCamera::Movement::friction = 7.0f;


FPVCamera::FPVCamera() :
    m_velocity(chrono::Vector()),
    m_movementSpeed(20.0f),
    m_pitch(0.0f),
    m_yaw(0.0f) {

    m_camera.near = 0.5f;
    m_camera.far = 300.0f;
    m_camera.verticalFOV = 44.7f;
    m_camera.position = {0.0, 0.0, 3.5};
    m_camera.up = {0, 1, 0};
    m_camera.lookAt = {0, 0, -1};

    syncPitchYawWithLookAt();

    // todo
    //Cameras_registerHandleTo(m_camera, "first_person_view");
}


void FPVCamera::handleInput(double dt) {
    ImGui::Begin("Before");
    ImGui::Text("m_pitch: %f, m_yaw: %f", m_pitch, m_yaw);
    ImGui::Text("m_pos: %f, %f, %f", m_camera.position.x(), m_camera.position.y(), m_camera.position.z());
    chrono::Vector& lookAt = m_camera.lookAt;
    ImGui::Text("lookAt: %f, %f, %f", lookAt.x(), lookAt.y(), lookAt.z());
    ImGui::End();

    handleMovementInput(dt);
    handleZoomInput(dt);
    handleDirectionInput();
}


void FPVCamera::update(double dt) {
    m_velocity *= 1.0 / (1.0 + (dt * Movement::friction));
    m_camera.position += m_velocity * dt;
}


void FPVCamera::handleMovementInput(double dt) {
    using namespace chrono;

    m_camera.lookAt.Normalize();

    Vector addedVelocity = Vector();

    Vector horizLookAt = {m_camera.lookAt.x(), 0.0, m_camera.lookAt.z()};
    horizLookAt.Normalize();

    if(Input_isKeyDown(GLFW_KEY_E))
        addedVelocity += horizLookAt;

    if(Input_isKeyDown(GLFW_KEY_D))
        addedVelocity -= horizLookAt;

    if(Input_isKeyDown(GLFW_KEY_F))
        addedVelocity += horizLookAt.Cross(VECT_Y);

    if(Input_isKeyDown(GLFW_KEY_S))
        addedVelocity -= horizLookAt.Cross(VECT_Y);

    if(Input_isKeyDown(GLFW_KEY_SPACE))
        addedVelocity.y() += 1.0;

    if(Input_isKeyDown(GLFW_KEY_LEFT_SHIFT))
        addedVelocity.y() -= 1.0;

    if(addedVelocity.Normalize())
        m_velocity += addedVelocity * m_movementSpeed * dt;
}


void FPVCamera::handleDirectionInput() {
    glm::ivec2 mouseDelta = Input_getMouseDelta();

    m_yaw += mouseDelta.x * Sensitivity::lookAround;
    m_pitch -= mouseDelta.y * Sensitivity::lookAround;
    clampPitchYaw();

    syncLookAtWithPitchYaw();
}


void FPVCamera::handleZoomInput(double dt) {
}


void FPVCamera::clampPitchYaw() {
    m_yaw -= 360.0f * floor(m_yaw / 360.0f);
    m_pitch = m_pitch < -89.9f ? -89.9f : (m_pitch > 89.9f ? 89.9f : m_pitch);
}


void FPVCamera::syncLookAtWithPitchYaw() {
    using namespace chrono;

    Vector result;

    // Since a yaw of 0 corresponds to -Z, we need to offset m_yaw by -90 degrees for trig calculations to work
    float yawOffset = m_yaw - 90.0f;

    result.x() = cos(CH_C_DEG_TO_RAD * m_pitch) * cos(CH_C_DEG_TO_RAD * yawOffset);
    result.y() = sin(CH_C_DEG_TO_RAD * m_pitch);
    result.z() = cos(CH_C_DEG_TO_RAD * m_pitch) * sin(CH_C_DEG_TO_RAD * yawOffset);
    result.Normalize();

    m_camera.lookAt = result;
}


void FPVCamera::syncPitchYawWithLookAt() {
    using namespace chrono;

    m_camera.lookAt.Normalize();

    // Horizontal component of the look-at vector
    Vector horizLookAt = Vector(m_camera.lookAt.x(), 0.0, m_camera.lookAt.z()).GetNormalized();
    m_yaw = CH_C_RAD_TO_DEG * acos(horizLookAt.Dot(-VECT_Z)) * (horizLookAt.x() < 0.0 ? -1 : 1);

    // Vertical component of look-at vector
    m_pitch = 90.0f - CH_C_RAD_TO_DEG * acos(m_camera.lookAt.Dot(VECT_Y));
}


} // namespace Graphics
} // namespace F9Sim
