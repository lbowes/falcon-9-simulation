#include "Scene.h"
#include "cameras/CameraSystem.h"

#include "../input/HWEventReceiver.h"


Scene::Scene(irr::scene::ISceneManager& sceneMgr, irr::video::IVideoDriver& vidDriver) :
    mCamerasHaveFocus(false),
    mSimModel(sceneMgr, vidDriver) {

    CameraSystem::init(sceneMgr);
}


void Scene::handleInput(double dt) {
    //if(Input::HWEventReceiver::isKeyPressed(irr::KEY_KEY_C))


    if(mCamerasHaveFocus)
        CameraSystem::handleInput();

    // temp
    testCam.handleInput(dt);
}


void Scene::update(float aspectRatio, double dt) {
    CameraSystem::setAspectRatio(aspectRatio);
    CameraSystem::updateIrrlichtCamera();

    mSimModel.updateAllNodeTransforms();

    // todo
    mSimModel.offsetAllNodePositionsBy(-CameraSystem::getActiveCameraPos());
    // temp
    testCam.update(dt);
}
