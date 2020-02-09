#ifndef CAMERASYSTEM_H_
#define CAMERASYSTEM_H_

#include "CameraBaseState.h"

#include <ICameraSceneNode.h>
#include <ISceneManager.h>

#include <map>


class CameraSystem {
private:
    static bool mInitialised;
    static float mAspectRatio;
    static irr::scene::ICameraSceneNode* mIrrlichtCam;
    static std::map<std::string, CameraBaseState*> mCameraStateHandles;
    static std::map<std::string, CameraBaseState*>::iterator mActiveCameraState;

public:
    CameraSystem() = delete;
    ~CameraSystem() = default;

    static void init(irr::scene::ISceneManager& sceneMgrHandle);
    static void handleInput();
    static void setScreenAspectRatio(float aspectRatio);
    static void updateIrrlichtCamera();
    static void registerHandleTo(CameraBaseState* cameraState, const std::string& name);
};

#endif // CAMERASYSTEM_H_

