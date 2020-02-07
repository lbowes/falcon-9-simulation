#ifndef CAMERASYSTEM_H_
#define CAMERASYSTEM_H_

#include <ICameraSceneNode.h>
#include <ISceneManager.h>


class CameraSystem {
private:
    float mAspectRatio;
    irr::scene::ICameraSceneNode* mInternalIrrlichtCam;

public:
    CameraSystem(irr::scene::ISceneManager& sceneMgrHandle);
    ~CameraSystem() = default;

    void handleInput(float frameTime_s);
    void updateAspectRatio(float aspectRatio);
    void update();
};

#endif // CAMERASYSTEM_H_

