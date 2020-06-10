#ifndef F9SIM_GRAPHICS_CAMERASYSTEM_H_
#define F9SIM_GRAPHICS_CAMERASYSTEM_H_
#pragma once

#include "CameraBaseState.h"

#include <vector>


namespace F9Sim {
namespace Graphics {


struct CameraHandle {
    std::string name;
    const CameraBaseState* handle;
};


bool sInitialised;
float sAspectRatio;
irr::scene::ICameraSceneNode* sIrrlichtCam;
std::vector<CameraHandle> sCameraHandles;
CameraHandle sActiveCamera;

void init(irr::scene::ISceneManager& sceneMgrHandle);
void handleInput();
void setAspectRatio(float aspectRatio);
void updateIrrlichtCamera();
void registerHandleTo(CameraBaseState& cameraState, const std::string& name);
chrono::ChVector<> getActiveCameraPos();


} // namespace Graphics
} // namespace F9Sim


#endif // F9SIM_GRAPHICS_CAMERASYSTEM_H_

