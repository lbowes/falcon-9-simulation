#include "SimulationModel.h"
#include "cameras/CameraSystem.h"

#include "../input/HWEventReceiver.h"


SimulationModel::SimulationModel(irr::scene::ISceneManager& sceneMgr) :
    mCamerasHaveFocus(false),
    mScene(sceneMgr) {

    CameraSystem::init(sceneMgr);
}


void SimulationModel::handleInput(double dt) {
    //if(Input::HWEventReceiver::isKeyPressed(irr::KEY_KEY_C))


    if(mCamerasHaveFocus)
        CameraSystem::handleInput();

    // temp
    testCam.handleInput(dt);
}


void SimulationModel::update(float aspectRatio, double dt) {
    CameraSystem::setAspectRatio(aspectRatio);
    CameraSystem::updateIrrlichtCamera();

    mScene.updateAllNodeTransforms();

    // todo
    mScene.offsetAllNodePositionsBy(-CameraSystem::getActiveCameraPos());
    // temp
    testCam.update(dt);
}
