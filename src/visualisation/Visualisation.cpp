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

    mGUI = std::make_unique<GUI>(*mDevice);
    mSimulationModel = std::make_unique<SimulationModel>(*mDevice->getSceneManager());

    mEventReceiver.addReceiver(&mHWInput);
    mEventReceiver.addReceiver(&mGUI->getEventReceiver());

    // temp
    rt = mVidDriver->addRenderTargetTexture(irr::core::dimension2d<irr::u32>(800, 800), "RTT1");
    // temp
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

        ImGui::ShowDemoWindow();
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
    mSimulationModel->update(getAspectRatio(), frameTime_s);

    Input::HWEventReceiver::update();
}


float Visualisation::getAspectRatio() {
    irr::core::recti viewport = mVidDriver->getViewPort();
    return static_cast<float>(viewport.getWidth()) / viewport.getHeight();
}


void Visualisation::render() {
    using namespace irr;

    mVidDriver->beginScene(true, true, video::SColor(255, 0, 0, 0));

    // Render everything to the render target
    mVidDriver->setRenderTarget(rt, true, true, video::SColor(0, 0, 0, 255));
    mSceneManager->drawAll();

    mVidDriver->setRenderTarget(0, true, true, 0);
    mGUI->render(rt);

    mVidDriver->endScene();
}


void Visualisation::close() {
    mDevice->closeDevice();
    mDevice->run();
    mDevice->drop();
}

