#ifndef FPVCAMERA_H_
#define FPVCAMERA_H_

#include "CameraBaseState.h"


class FPVCamera {
private:
    struct Sensitivity {
        static const float mZoom;
        static const float mLookAround;
        static const float mAdjustSpeed;
    } mSensitivity;

    struct Movement {
        static const float mMinSpeed;
        static const float mMaxSpeed;
        static const float mFriction;
    } mMovement;

    CameraBaseState mCameraBase;

    chrono::ChVector<> mVelocity;
    float mMovementSpeed;
    float mPitch;
    float mYaw;

public:
    FPVCamera();
    ~FPVCamera() = default;

    void handleInput();
    void registerWithCameraSystem();
    void update();
};

#endif // FPVCAMERA_H_
