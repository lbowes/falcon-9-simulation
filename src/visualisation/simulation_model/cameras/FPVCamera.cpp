#include "FPVCamera.h"
#include "../../input/HWEventReceiver.h"
#include "CameraSystem.h"

#include <IMGUI/imgui.h>
#include <algorithm>
#include <chrono/core/ChMathematics.h>
#include <iostream>


// Magic constants
const float FPVCamera::Sensitivity::mZoom = 0.1f;
const float FPVCamera::Sensitivity::mLookAround = 0.45f;
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
    mCameraBase.position = {3.5f, 0.5f, 0};
    mCameraBase.up = {0, 1, 0};
    mCameraBase.lookAt = {-1, 0, 0};

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

    mCameraBase.lookAt = recalcLookAtVec(mPitch, mYaw);
}


void FPVCamera::handleZoomInput(double dt) {
}


void FPVCamera::clampPitchYaw() {
    mYaw -= 360.0f * floor(mYaw / 360.0f);
    mPitch = std::clamp(mPitch, -89.9f, 89.9f);
}


chrono::ChVector<> FPVCamera::recalcLookAtVec(float pitch, float yaw) const {
    using namespace chrono;

    ChVector<> result;
    result.x() = cos(CH_C_DEG_TO_RAD * pitch) * cos(CH_C_DEG_TO_RAD * yaw);
    result.y() = sin(CH_C_DEG_TO_RAD * pitch);
    result.z() = cos(CH_C_DEG_TO_RAD * pitch) * sin(CH_C_DEG_TO_RAD * yaw);
    result.Normalize();

    return result;
}
