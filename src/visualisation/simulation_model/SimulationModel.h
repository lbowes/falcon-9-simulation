#ifndef SIMULATIONMODEL_H_
#define SIMULATIONMODEL_H_

#include "Scene.h"
#include "cameras/FPVCamera.h"

#include <ISceneManager.h>
#include <IrrlichtDevice.h>


class SimulationModel {
private:
    bool mCamerasHaveFocus;
    Scene mScene;
    FPVCamera testCam;

public:
    SimulationModel(irr::scene::ISceneManager& sceneMgr);
    ~SimulationModel() = default;

    void handleInput(double dt);
    void update(float aspectRatio, double dt);
};

#endif // SIMULATIONMODEL_H_
