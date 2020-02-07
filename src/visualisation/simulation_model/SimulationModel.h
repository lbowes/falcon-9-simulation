#ifndef SIMULATIONMODEL_H_
#define SIMULATIONMODEL_H_

#include "Scene.h"
#include "cameras/CameraSystem.h"

#include <ISceneManager.h>
#include <IrrlichtDevice.h>


class SimulationModel {
private:
    irr::scene::ISceneManager& mSceneMgrHandle;
    Scene mScene;
    CameraSystem mCameraSystem;

public:
    SimulationModel(irr::scene::ISceneManager& sceneMgr);
    ~SimulationModel() = default;

    void initWith(irr::IrrlichtDevice& device);
    void handleInput(double frameTime_s);
    void update(double frameTime_s, float aspectRatio);

private:
    void offsetAllSceneNodesByCamPos();
};

#endif // SIMULATIONMODEL_H_

