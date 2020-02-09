#include "FPVCamera.h"
#include "../../input/HWEventReceiver.h"
#include "CameraSystem.h"


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
    mCameraBase.position = {3.5f, 0.5f, 0};
    mCameraBase.up = {0, 1, 0};
    mCameraBase.lookAt = {-1, 0, 0};

    CameraSystem::registerHandleTo(mCameraBase, "first_person_view");
}


void FPVCamera::handleInput(double dt) {
    handleMovementInput(dt);
    handleZoomInput(dt);
    handleDirectionInput(dt);
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


void FPVCamera::handleDirectionInput(double dt) {
    using namespace irr::core;

    vector2di mouseDelta = Input::HWEventReceiver::getMouse().getDelta_scr();

    mYaw -= mouseDelta.X * Sensitivity::mLookAround;
    mPitch -= mouseDelta.Y * Sensitivity::mLookAround;
    //clampPitchYaw();

    //mLookAtDir_world = recalcLookAtVec(mPitch, mYaw);

    //temp
    //mCameraBase.lookAt = -mCameraBase.position;
}


void FPVCamera::handleZoomInput(double dt) {
}
