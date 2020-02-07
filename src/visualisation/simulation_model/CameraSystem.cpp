#include "CameraSystem.h"


CameraSystem::CameraSystem() :
    mAspectRatio(1.0f) {
}


void CameraSystem::handleInput() {
}


void CameraSystem::updateAspectRatio(float aspectRatio) {
    mAspectRatio = aspectRatio;
}


void CameraSystem::update() {
}
