#include "Visualisation.h"

#include <IMGUI/imgui.h>
#include <IVideoDriver.h>
#include <iostream>


Visualisation::Visualisation() :
    mDevice(nullptr),
    mVidDriver(nullptr) {

    initDevice();

    mVidDriver = mDevice->getVideoDriver();
    mSceneManager = mDevice->getSceneManager();

    const irr::core::dimension2du monitorRes = getMonitorResolution();
    mGUI = std::make_unique<GUI>(*mDevice, monitorRes);
    mSimulationModel = std::make_unique<SimulationModel>(*mDevice->getSceneManager());

    mEventReceiver.addReceiver(&mHWInput);
    mEventReceiver.addReceiver(&mGUI->getEventReceiver());
}


Visualisation::~Visualisation() {
    close();
}


void Visualisation::initDevice() {
    irr::SIrrlichtCreationParameters params;
    params.DriverType = irr::video::EDT_OPENGL;
    params.AntiAlias = true;
    params.WindowSize = getMonitorResolution() / 2;
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
}


irr::core::dimension2du Visualisation::getMonitorResolution() const {
    irr::IrrlichtDevice* temp = irr::createDevice(irr::video::EDT_NULL);
    irr::core::dimension2du monitorRes = temp->getVideoModeList()->getDesktopResolution();
    temp->drop();
    return monitorRes;
}


void Visualisation::run() {
    unsigned lastTime = mDevice->getTimer()->getRealTime();

    while(mDevice->run()) {
        const unsigned now = mDevice->getTimer()->getRealTime();
        const float frameTime_s = static_cast<float>(now - lastTime) / 1000.0f;

        mGUI->start();

        if(mDevice->isWindowFocused())
            handleInput(frameTime_s);

        update(frameTime_s);
        render();

        lastTime = now;
    }
}


void Visualisation::handleInput(double frameTime_s) {
    using namespace Input;

    if(HWEventReceiver::isKeyPressed(irr::KEY_ESCAPE))
        close();

    mSimulationModel->handleInput(frameTime_s);
}


void Visualisation::update(double frameTime_s) {
    const float aspect = mGUI->getSimViewWindowAspectRatio();
    mSimulationModel->update(aspect, frameTime_s);

    Input::HWEventReceiver::update();
}


void Visualisation::render() {
    // Bind the simulation view render target and render the scene to it
    mVidDriver->setRenderTarget(&mGUI->getSimViewRenderTarget());
    mVidDriver->beginScene();
    mSceneManager->drawAll();

    // Swap back to the main render target and render the GUI
    mVidDriver->setRenderTarget(irr::video::ERT_FRAME_BUFFER);
    mGUI->render();
    mVidDriver->endScene();
}


void Visualisation::close() {
    mDevice->closeDevice();
    mDevice->run();
    mDevice->drop();
}

