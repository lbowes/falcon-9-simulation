#ifndef VISUALISATION_H_
#define VISUALISATION_H_

#include "graphics/gui/GUI.h"
#include "input/MasterEventReceiver.h"

#include <chrono_irrlicht/ChIrrWizard.h>


class Visualisation {
private:
    irr::IrrlichtDevice* mDevice;
    irr::video::IVideoDriver* mVidDriver;
    irr::scene::ISceneManager* mSceneMgr;

    Input::HWEventReceiver mHWInput;
    Input::MasterEventReceiver mEventReceiver;
    Graphics::GUI mGUI;

public:
    Visualisation();
    ~Visualisation() = default;

    void run();

private:
    irr::core::dimension2du getMonitorResolution() const;
    void initIrrlicht();
    void initImGui();
    void setImGuiStyle();
    void render();
    void update();
};

#endif // VISUALISATION_H_
