#ifndef VISUALISATION_H_
#define VISUALISATION_H_

#include "input/MasterEventReceiver.h"

#include <IrrIMGUI/IrrIMGUI.h>
#include <chrono_irrlicht/ChIrrWizard.h>


class Visualisation {
private:
    irr::IrrlichtDevice* mDevice;
    irr::video::IVideoDriver* mVidDriver;
    irr::scene::ISceneManager* mSceneMgr;
    IrrIMGUI::IIMGUIHandle* mImGuiHandle;

    Input::HWEventReceiver mHWInput;
    Input::MasterEventReceiver mEventReceiver;
    IrrIMGUI::CIMGUIEventReceiver mImGuiEventReceiver;

public:
    Visualisation();
    ~Visualisation() = default;

    void run();

private:
    void initIrrlicht();
    void initImGui();
    void setImGuiStyle();
    void render();
    void update();
};

#endif // VISUALISATION_H_
