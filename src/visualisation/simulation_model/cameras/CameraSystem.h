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
    static bool sInitialised;
    static float sAspectRatio;
    static irr::scene::ICameraSceneNode* sIrrlichtCam;
    static std::vector<CameraHandle> sCameraHandles;
    static CameraHandle sActiveCamera;

public:
    CameraSystem() = delete;
    ~CameraSystem() = default;

    static void init(irr::scene::ISceneManager& sceneMgrHandle);
    static void handleInput();
    static void setAspectRatio(float aspectRatio);
    static void updateIrrlichtCamera();
    static void registerHandleTo(CameraBaseState& cameraState, const std::string& name);
    static chrono::ChVector<> getActiveCameraPos();
};

#endif // CAMERASYSTEM_H_

