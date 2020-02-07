#ifndef FPVCAMERA_H_
#define FPVCAMERA_H_

#include "CameraBaseState.h"


class FPVCamera {
private:
    static const float mZoomSensitivity;
    static const float mLookAroundSensitivity;
    static const float mSpeedAdjustSensitivity;
    static const float mMinMoveSpeed;
    static const float mMaxMoveSpeed;
    static const float mMoveFriction;

    CameraBaseState mCameraBase;

    chrono::ChVector<> mVelocity;
    float mMovementSpeed;
    float mPitch;
    float mYaw;

public:
    FPVCamera();
    ~FPVCamera() = default;

    void handleInput();
    void update();
};

#endif // FPVCAMERA_H_
