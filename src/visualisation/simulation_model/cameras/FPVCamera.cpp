#include "FPVCamera.h"
#include "CameraSystem.h"


// Magic constants
const float FPVCamera::Sensitivity::mZoom = 0.1f;
const float FPVCamera::Sensitivity::mLookAround = 0.05f;
const float FPVCamera::Sensitivity::mAdjustSpeed = 200.0f;
const float FPVCamera::Movement::mMinSpeed = 6.4f;
const float FPVCamera::Movement::mMaxSpeed = 1000.0f;
const float FPVCamera::Movement::mFriction = 7.0f;


FPVCamera::FPVCamera() {
    mCameraBase.nearValue = 0.1f;
    mCameraBase.farValue = 300.0f;
    mCameraBase.verticalFOV = 44.7f;
    mCameraBase.position = {0, 2, 0};
    mCameraBase.up = {0, 1, 0};
    mCameraBase.lookAt = {-1, 0, 0};
}


void FPVCamera::handleInput() {
}


void FPVCamera::registerWithCameraSystem() {
    CameraSystem::registerHandleTo(&mCameraBase, "first_person_view");
}


void FPVCamera::update() {
}

