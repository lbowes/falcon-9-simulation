#ifndef VISUALISATION_H_
#define VISUALISATION_H_

#include "GUI.h"
#include "input/MasterEventReceiver.h"
#include "simulation_model/SimulationModel.h"

#include <memory>


class Visualisation {
private:
    irr::IrrlichtDevice* mDevice;
    irr::video::IVideoDriver* mVidDriver;
    irr::scene::ISceneManager* mSceneManager;

    Input::HWEventReceiver mHWInput;
    Input::MasterEventReceiver mEventReceiver;
    std::unique_ptr<GUI> mGUI;
    std::unique_ptr<SimulationModel> mSimulationModel;

public:
    Visualisation();
    ~Visualisation();

    void run();

private:
    void handleInput(double frameTime_s);
    void update(double frameTime_s);
    irr::core::dimension2du getMonitorResolution() const;
    void initDevice();
    void render();
    void update();
    void close();
};

#endif // VISUALISATION_H_

