#include "Visualisation.h"

#include <IMGUI/imgui.h>
#include <iostream>

Visualisation::Visualisation() {
    using namespace irr;

    SIrrlichtCreationParameters params;
    params.DriverType = video::EDT_OPENGL;

    IrrlichtDevice* temp = createDevice(video::EDT_NULL);
    core::dimension2du monitorRes = temp->getVideoModeList()->getDesktopResolution();
    temp->drop();
    params.WindowSize = monitorRes / 2;

    params.Bits = 16;
    params.Fullscreen = false;
    params.Stencilbuffer = true;
    params.Vsync = true;
    params.EventReceiver = &mEventReceiver;

    mDevice = createDeviceEx(params);

    if(!mDevice) {
        printf("Error creating Irrlicht device\n");
        std::cin.get();
        return;
    }

    mDevice->setWindowCaption(L"Falcon 9 Simulation");

    mVidDriver = mDevice->getVideoDriver();
    mSceneMgr = mDevice->getSceneManager();

    mEventReceiver.addReceiver(&mHWInput);
    mEventReceiver.addReceiver(&mImGuiEventReceiver);

    IrrIMGUI::SIMGUISettings settings;
    settings.mIsGUIMouseCursorEnabled = false;

    mImGuiHandle = IrrIMGUI::createIMGUI(mDevice, &mImGuiEventReceiver, &settings);
}


void Visualisation::run() {
    unsigned lastTime = mDevice->getTimer()->getRealTime();

    while(mDevice->run()) {
        const unsigned now = mDevice->getTimer()->getRealTime();
        const float frameTime_s = static_cast<float>(now - lastTime) / 1000.0f;

        //mImGuiHandle->startGUI();
        //ImGui::Begin("Test window");
        // handleInput(frameTime_s);
        // update(frameTime_s);
        render();

        lastTime = now;
    }
}


void Visualisation::render() {
    mVidDriver->beginScene(true, true, irr::video::SColor(255, 100, 70, 0));

    // mGUILayer.render();
    // mModel->render(mCameraSystem->getCurrentSimCamera().getPosition_world());
    //mImGuiHandle->drawAll();

    mVidDriver->endScene();
}
