#ifndef SCENE_H_
#define SCENE_H_

#include "SimulationModel.h"
#include "cameras/FPVCamera.h"

#include <ISceneManager.h>
#include <IrrlichtDevice.h>


class Scene {
private:
    bool mCamerasHaveFocus;
    SimulationModel mSimModel;
    FPVCamera testCam;

public:
    Scene(irr::scene::ISceneManager& sceneMgr, irr::video::IVideoDriver& vidDriver);
    ~Scene() = default;

    void handleInput(double dt);
    void update(float aspectRatio, double dt);
};


#endif // SCENE_H_

