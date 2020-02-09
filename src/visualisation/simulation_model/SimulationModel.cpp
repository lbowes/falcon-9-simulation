#include "SimulationModel.h"
#include "cameras/CameraSystem.h"


SimulationModel::SimulationModel(irr::scene::ISceneManager& sceneMgr) :
    mSceneMgrHandle(sceneMgr),
    mScene(sceneMgr) {

    CameraSystem::init(sceneMgr);
}


void SimulationModel::handleInput(double dt) {
    CameraSystem::handleInput();
    // temp
    testCam.handleInput(dt);
}


void SimulationModel::update(float aspectRatio, double dt) {
    CameraSystem::setScreenAspectRatio(aspectRatio);
    CameraSystem::updateIrrlichtCamera();

    mScene.updateAllNodeTransforms();

    // todo
    mScene.applyCameraPosOffset(CameraSystem::getActiveCameraPos());

    // temp
    testCam.update(dt);
}
