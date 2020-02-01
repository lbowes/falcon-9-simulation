#ifndef VISUALISATION_H_
#define VISUALISATION_H_

#include "input/MasterEventReceiver.h"

#include <chrono_irrlicht/ChIrrWizard.h>

class Visualisation {
private:
    irr::IrrlichtDevice* mDevice;
    irr::video::IVideoDriver* mVidDriver;
    irr::scene::ISceneManager* mSceneMgr;
    Input::MasterEventReceiver mEventReceiver;

public:
    Visualisation();
    ~Visualisation() = default;

    void run();

private:
    void render();
    void update();
};

#endif // VISUALISATION_H_
