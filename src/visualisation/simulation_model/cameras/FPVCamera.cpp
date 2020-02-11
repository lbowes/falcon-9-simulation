#include "FPVCamera.h"
#include "../../input/HWEventReceiver.h"
#include "CameraSystem.h"

#include <IMGUI/imgui.h>
#include <algorithm>
#include <chrono/core/ChMathematics.h>
#include <iostream>


// Magic constants
const float FPVCamera::Sensitivity::mZoom = 0.1f;
const float FPVCamera::Sensitivity::mLookAround = 0.05f;
const float FPVCamera::Sensitivity::mAdjustSpeed = 200.0f;
const float FPVCamera::Movement::mMinSpeed = 6.4f;
const float FPVCamera::Movement::mMaxSpeed = 1000.0f;
const float FPVCamera::Movement::mFriction = 7.0f;


FPVCamera::FPVCamera() :
    mVelocity(chrono::ChVector<>()),
    mMovementSpeed(20.0f),
    mPitch(0.0f),
    mYaw(0.0f) {

    mCameraBase.nearValue = 0.1f;
    mCameraBase.farValue = 300.0f;
    mCameraBase.verticalFOV = 44.7f;
    mCameraBase.position = {0.0, 0.5, 3.5};
    mCameraBase.up = {0, 1, 0};
    mCameraBase.lookAt = {0.6, 0, -1};

    syncPitchYawWithLookAt();

    CameraSystem::registerHandleTo(mCameraBase, "first_person_view");
}


void FPVCamera::handleInput(double dt) {
    handleMovementInput(dt);
    handleZoomInput(dt);
    handleDirectionInput();
}


void FPVCamera::update(double dt) {
    mVelocity *= 1.0 / (1.0 + (dt * Movement::mFriction));
    mCameraBase.position += mVelocity * dt;
}


void FPVCamera::handleMovementInput(double dt) {
    using namespace irr;
    using namespace irr::core;
    using namespace chrono;
    using Input::HWEventReceiver;

    mCameraBase.lookAt.Normalize();

    ChVector<> addedVelocity = ChVector<>();
    const ChVector<> horizLookAt = {mCameraBase.lookAt.x(), 0.0, mCameraBase.lookAt.z()};

    if(HWEventReceiver::isKeyPressed(KEY_KEY_E))
        addedVelocity += horizLookAt;

    if(Input::HWEventReceiver::isKeyPressed(KEY_KEY_D))
        addedVelocity -= horizLookAt;

    if(Input::HWEventReceiver::isKeyPressed(KEY_KEY_S))
        addedVelocity += horizLookAt.Cross(VECT_Y);

    if(Input::HWEventReceiver::isKeyPressed(KEY_KEY_F))
        addedVelocity -= horizLookAt.Cross(VECT_Y);

    if(Input::HWEventReceiver::isKeyPressed(KEY_SPACE))
        addedVelocity.y() += 1.0;

    if(Input::HWEventReceiver::isKeyPressed(KEY_LSHIFT))
        addedVelocity.y() -= 1.0;

    if(addedVelocity.Normalize())
        mVelocity += addedVelocity * mMovementSpeed * dt;
}


void FPVCamera::handleDirectionInput() {
    using namespace irr::core;

    vector2di mouseDelta = Input::MouseState::getDelta_scr();

    mYaw -= mouseDelta.X * Sensitivity::mLookAround;
    mPitch -= mouseDelta.Y * Sensitivity::mLookAround;
    clampPitchYaw();

    syncLookAtWithPitchYaw();

    ImGui::Begin("camera info");
    ImGui::Text("mPitch: %f, mYaw: %f", mPitch, mYaw);
    const chrono::ChVector<>& lookAt = mCameraBase.lookAt;
    ImGui::Text("lookAt: %f, %f, %f", lookAt.x(), lookAt.y(), lookAt.z());
    ImGui::Text("mouseDelta: %d, %d", mouseDelta.X, mouseDelta.Y);
    vector2di mousePos = Input::MouseState::getPosition_scr();
    ImGui::Text("mousePosition: %d, %d", mousePos.X, mousePos.Y);
    ImGui::End();
}


void FPVCamera::handleZoomInput(double dt) {
}


void FPVCamera::clampPitchYaw() {
    mYaw -= 360.0f * floor(mYaw / 360.0f);
    mPitch = std::clamp(mPitch, -89.9f, 89.9f);
}


void FPVCamera::syncLookAtWithPitchYaw() {
    using namespace chrono;

    ChVector<> result;

    // Since a yaw of 0 corresponds to -Z, we need to offset mYaw by -90 degrees for trig calculations to work
    float yawOffset = mYaw - 90.0f;

    result.x() = cos(CH_C_DEG_TO_RAD * mPitch) * cos(CH_C_DEG_TO_RAD * yawOffset);
    result.y() = sin(CH_C_DEG_TO_RAD * mPitch);
    result.z() = cos(CH_C_DEG_TO_RAD * mPitch) * sin(CH_C_DEG_TO_RAD * yawOffset);
    result.Normalize();

    mCameraBase.lookAt = result;
}


void FPVCamera::syncPitchYawWithLookAt() {
    using namespace chrono;

    mCameraBase.lookAt.Normalize();

    // Horizontal component of the look-at vector
    ChVector<> horizLookAt = ChVector<>(mCameraBase.lookAt.x(), 0.0, mCameraBase.lookAt.z()).GetNormalized();
    mYaw = CH_C_RAD_TO_DEG * acos(horizLookAt.Dot(-VECT_Z)) * (horizLookAt.x() < 0.0 ? -1 : 1);

    mPitch = 90.0 - CH_C_RAD_TO_DEG * acos(mCameraBase.lookAt.Dot(VECT_Y));
}
