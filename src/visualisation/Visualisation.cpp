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
    mVertFlipScreenQuad = std::make_unique<VertFlipScreenQuad>(*mVidDriver, monitorRes);
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
    mVidDriver->beginScene();

    // Get the image of the scene onto the render target texture in the fullscreen quad
    mVidDriver->setRenderTarget(&mVertFlipScreenQuad->getRenderTarget(), true, true, irr::video::SColor(255, 0, 0, 0));
    mSceneManager->drawAll();

    // Get the contents of the fullscreen quad (vertically flipped version of the scene's contents) into the GUI's
    // simulation view
    mVidDriver->setRenderTarget(&mGUI->getSimViewRenderTarget());
    mVertFlipScreenQuad->drawAll();

    // Get the contents of the GUI (complete sim view and other windows) onto the monitor
    mVidDriver->setRenderTarget(irr::video::ERT_FRAME_BUFFER);
    mGUI->drawAll();

    mVidDriver->endScene();
}


void Visualisation::close() {
    mDevice->closeDevice();
    mDevice->run();
    mDevice->drop();
}
