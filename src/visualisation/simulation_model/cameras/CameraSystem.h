#ifndef CAMERASYSTEM_H_
#define CAMERASYSTEM_H_

#include "CameraBaseState.h"

#include <ICameraSceneNode.h>
#include <ISceneManager.h>

#include <vector>


class CameraSystem {
public:
    struct CameraHandle {
        std::string name;
        const CameraBaseState* handle;
    };

private:
    static bool mInitialised;
    static float mAspectRatio;
    static irr::scene::ICameraSceneNode* mIrrlichtCam;
    static std::vector<CameraHandle> mCameraHandles;
    static CameraHandle mActiveCamera;

public:
    CameraSystem() = delete;
    ~CameraSystem() = default;

    static void init(irr::scene::ISceneManager& sceneMgrHandle);
    static void handleInput();
    static void setScreenAspectRatio(float aspectRatio);
    static void updateIrrlichtCamera();
    static void registerHandleTo(CameraBaseState& cameraState, const std::string& name);
    static chrono::ChVector<> getActiveCameraPos();
};

#endif // CAMERASYSTEM_H_

