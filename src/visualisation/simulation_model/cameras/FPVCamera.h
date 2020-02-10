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

    void handleInput(double dt);
    void update(double dt);

private:
    void handleMovementInput(double dt);
    void handleDirectionInput();
    void handleZoomInput(double dt);
    void clampPitchYaw();
    chrono::ChVector<> recalcLookAtVec(float pitch, float yaw) const;
};

#endif // FPVCAMERA_H_
