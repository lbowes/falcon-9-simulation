#include "SimulationModel.h"


SimulationModel::SimulationModel(irr::scene::ISceneManager& sceneMgr) :
    mSceneMgrHandle(sceneMgr),
    mScene(sceneMgr) {
}


void SimulationModel::handleInput(double frameTime_s) {
    // todo
}


void SimulationModel::update(double frameTime_s, float aspectRatio) {
    mCameraSystem.updateAspectRatio(aspectRatio);

    mScene.updateAllNodeTransforms();
    mScene.applyCameraPosOffset(chrono::ChVector<>() /* mCameraSystem.getActiveCameraPos() */);
}
