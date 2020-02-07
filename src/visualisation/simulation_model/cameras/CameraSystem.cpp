#include "CameraSystem.h"


CameraSystem::CameraSystem(irr::scene::ISceneManager& sceneMgrHandle) :
    mAspectRatio(1.0f) {

    mInternalIrrlichtCam = sceneMgrHandle.addCameraSceneNode();
}


void CameraSystem::handleInput(float frameTime_s) {
}


void CameraSystem::updateAspectRatio(float aspectRatio) {
    mAspectRatio = aspectRatio;
}


void CameraSystem::update() {
}
