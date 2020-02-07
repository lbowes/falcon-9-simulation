#ifndef CAMERABASESTATE_H_
#define CAMERABASESTATE_H_

#include <chrono/core/ChVector.h>


struct CameraBaseState {
    float aspectRatio;
    float nearValue;
    float farValue;
    float verticalFOV;
    chrono::ChVector<> position;
    chrono::ChVector<> lookAt;
};

#endif // CAMERABASESTATE_H_
