#include "SimulationModel.h"
#include "cameras/CameraSystem.h"


SimulationModel::SimulationModel(irr::scene::ISceneManager& sceneMgr) :
    mSceneMgrHandle(sceneMgr),
    mScene(sceneMgr) {

    CameraSystem::init(sceneMgr);

    testCam.registerWithCameraSystem();
}


void SimulationModel::handleInput() {
    CameraSystem::handleInput();
}


void SimulationModel::update(float aspectRatio) {
    CameraSystem::setScreenAspectRatio(aspectRatio);
    CameraSystem::updateIrrlichtCamera();

    mScene.updateAllNodeTransforms();
    // mScene.applyCameraPosOffset(chrono::ChVector<>(2.5, -1, 0));

    // todo
    mScene.applyCameraPosOffset(mCameraSystem.getActiveCameraPos());
}
