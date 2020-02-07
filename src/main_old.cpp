#include <IEventReceiver.h>
#include <IMGUI/imgui.h>
#include <IVideoDriver.h>
#include <IrrIMGUI/IrrIMGUI.h>
#include <IrrlichtDevice.h>
#include <SIrrCreationParameters.h>
#include <chrono/core/ChVector.h>
#include <chrono_irrlicht/ChIrrTools.h>
#include <irrArray.h>
#include <irrlicht.h>
#include <vector2d.h>

namespace Input {

    class MouseState {
        friend class HWEventReceiver;

    public:
        enum Button : unsigned char {
            left = 0,
            middle,
            right
        };

    private:
        irr::core::vector2di mPosition_scr;
        irr::core::vector2di mDelta_scr;
        bool mButtonsPressed[3];
        float mWheelScroll;

    public:
        MouseState() :
            mPosition_scr(0, 0),
            mWheelScroll(0.0f) {

            mButtonsPressed[left] = mButtonsPressed[middle] = mButtonsPressed[right] = false;
        }

        ~MouseState() = default;

        const irr::core::vector2di& getPosition_scr() const {
            return mPosition_scr;
        }

        const irr::core::vector2di& getDelta_scr() const {
            return mDelta_scr;
        }

        bool isButtonPressed(Button b) const {
            return mButtonsPressed[b];
        }

        float getScroll() const {
            return mWheelScroll;
        }

        void setPosition(irr::core::vector2di newPosition_scr) {
            mPosition_scr = newPosition_scr;
        }
    };

    class HWEventReceiver : public irr::IEventReceiver {
    private:
        MouseState mMouseState;
        bool mKeysDown[irr::KEY_KEY_CODES_COUNT];
        bool mKeysClicked[irr::KEY_KEY_CODES_COUNT];
        irr::core::vector2df mMousePosition;
        irr::core::vector2di mCentreScreenPos_scr;

    public:
        HWEventReceiver() {
            for(unsigned key = 0; key < irr::KEY_KEY_CODES_COUNT; key++)
                mKeysDown[key] = mKeysClicked[key] = false;
        }

        ~HWEventReceiver() = default;

        void update(irr::core::vector2di centreScreenPos_scr) {
            mCentreScreenPos_scr = centreScreenPos_scr;
            mMouseState.mPosition_scr = centreScreenPos_scr;
            mMouseState.mDelta_scr.X = mMouseState.mDelta_scr.Y = 0;

            mMouseState.mWheelScroll = 0.0f;

            for(bool& k : mKeysClicked)
                k = false;
        }

        virtual bool OnEvent(const irr::SEvent& e) {
            using namespace irr;

            switch(e.EventType) {
            case EET_KEY_INPUT_EVENT: {
                mKeysDown[e.KeyInput.Key] = e.KeyInput.PressedDown;
                mKeysClicked[e.KeyInput.Key] = !e.KeyInput.PressedDown;
                break;
            }
            case EET_MOUSE_INPUT_EVENT: {
                switch(e.MouseInput.Event) {
                case EMIE_LMOUSE_PRESSED_DOWN: {
                    mMouseState.mButtonsPressed[MouseState::Button::left] = true;
                    break;
                }
                case EMIE_LMOUSE_LEFT_UP: {
                    mMouseState.mButtonsPressed[MouseState::Button::left] = false;
                    break;
                }
                case EMIE_MMOUSE_PRESSED_DOWN: {
                    mMouseState.mButtonsPressed[MouseState::Button::middle] = true;
                    break;
                }
                case EMIE_MMOUSE_LEFT_UP: {
                    mMouseState.mButtonsPressed[MouseState::Button::middle] = false;
                    break;
                }
                case EMIE_RMOUSE_PRESSED_DOWN: {
                    mMouseState.mButtonsPressed[MouseState::Button::right] = true;
                    break;
                }
                case EMIE_RMOUSE_LEFT_UP: {
                    mMouseState.mButtonsPressed[MouseState::Button::right] = false;
                    break;
                }
                case EMIE_MOUSE_MOVED: {
                    mMouseState.mPosition_scr.X = e.MouseInput.X;
                    mMouseState.mPosition_scr.Y = e.MouseInput.Y;

                    mMouseState.mDelta_scr.X = mMouseState.mPosition_scr.X - mCentreScreenPos_scr.X;
                    mMouseState.mDelta_scr.Y = mMouseState.mPosition_scr.Y - mCentreScreenPos_scr.Y;
                    break;
                }
                case EMIE_MOUSE_WHEEL: {
                    mMouseState.mWheelScroll = e.MouseInput.Wheel;
                    break;
                }
                case EMIE_LMOUSE_DOUBLE_CLICK:
                case EMIE_RMOUSE_DOUBLE_CLICK:
                case EMIE_MMOUSE_DOUBLE_CLICK:
                case EMIE_LMOUSE_TRIPLE_CLICK:
                case EMIE_RMOUSE_TRIPLE_CLICK:
                case EMIE_MMOUSE_TRIPLE_CLICK:
                    break;
                }
                break;
            }
            default:
                break;
            }

            return false;
        }

        bool isKeyPressed(irr::EKEY_CODE key) const {
            return mKeysDown[key];
        }

        bool isKeyReleased(irr::EKEY_CODE key) const {
            return mKeysClicked[key];
        }

        const MouseState& getMouse() const {
            return mMouseState;
        }

        MouseState& getMouse() {
            return mMouseState;
        }
    };


    class MasterEventReceiver : public irr::IEventReceiver {
    private:
        irr::core::array<irr::IEventReceiver*> mReceivers;

    public:
        MasterEventReceiver() = default;
        ~MasterEventReceiver() = default;


        void addReceiver(irr::IEventReceiver* receiver) {
            irr::s32 r = mReceivers.linear_search(receiver);
            if(r != -1)
                return;

            mReceivers.push_back(receiver);
        }


        void removeReceiver(irr::IEventReceiver* receiver) {
            irr::s32 r = mReceivers.linear_search(receiver);
            if(r != -1)
                mReceivers.erase(r);
        }


        virtual bool OnEvent(const irr::SEvent& e) {
            for(unsigned r = 0; r < mReceivers.size(); r++) {
                if(mReceivers[r]->OnEvent(e))
                    return true;
            }

            return false;
        }
    };

} // namespace Input


irr::IrrlichtDevice* mDevice;
irr::video::IVideoDriver* mVidDriver;
Input::HWEventReceiver mHWInput;
Input::MasterEventReceiver mEventReceiver;
IrrIMGUI::IIMGUIHandle* mImGuiHandle;
IrrIMGUI::CIMGUIEventReceiver mImGuiEventReceiver;
irr::scene::ISceneManager* mSceneMgr;
irr::scene::ISceneNode* engineSceneNode;
chrono::ChCoordsys<> engineTransform_world = chrono::ChCoordsys<>(chrono::ChVector<>(0.0f, 1.75f, 0.0f));
irr::scene::ICameraSceneNode* mCamera;
chrono::ChVector<> mCameraPos = chrono::ChVector<>(2.0f, 1.78f, 0.0f);
chrono::ChVector<> mCameraVelocity;
chrono::ChVector<> mCameraLookAtDir;
const float mCameraMovementFriction = 7.0f;
bool mCamerasHaveFocus = false;

irr::core::dimension2du getMonitorResolution() {
    irr::IrrlichtDevice* temp = irr::createDevice(irr::video::EDT_NULL);
    irr::core::dimension2du monitorRes = temp->getVideoModeList()->getDesktopResolution();
    temp->drop();
    return monitorRes;
}


void addEngineMesh() {
    irr::scene::IMesh* mesh = mSceneMgr->getMesh("../res/obj/Merlin1D.obj");
    engineSceneNode = mSceneMgr->addMeshSceneNode(mesh);
    engineSceneNode->setPosition(irr::core::vector3df(0.0f, 1.78f, 0.0f));
}

float getAspectRatio() {
    irr::core::recti viewport = mVidDriver->getViewPort();
    return static_cast<float>(viewport.getWidth()) / viewport.getHeight();
}

void initCamera() {
    mCamera = mSceneMgr->addCameraSceneNode();
    mCamera->setPosition({2.0f, 1.78f, 0.0f});
    mCamera->setUpVector({0, 1, 0});
    mCamera->setTarget({4, 0, 0});
    mCamera->setNearValue(0.1f);
    mCamera->setFarValue(300.0f);
    mCamera->setAspectRatio(getAspectRatio());
    mCamera->setFOV(irr::core::degToRad(44.7f));

    mSceneMgr->setActiveCamera(mCamera);
}


void init() {
    irr::SIrrlichtCreationParameters params;
    params.DriverType = irr::video::EDT_OPENGL;
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
    mDevice->maximizeWindow();

    mVidDriver = mDevice->getVideoDriver();
    mSceneMgr = mDevice->getSceneManager();

    // ======= IMGUI INITIALISATION =======
    ImGui::CreateContext();
    IrrIMGUI::SIMGUISettings settings;
    settings.mIsGUIMouseCursorEnabled = false;
    mImGuiHandle = IrrIMGUI::createIMGUI(mDevice, &mImGuiEventReceiver, &settings);

    mEventReceiver.addReceiver(&mHWInput);
    mEventReceiver.addReceiver(&mImGuiEventReceiver);

    addEngineMesh();
    initCamera();
}

void resetCursorToCenterScr(irr::gui::ICursorControl* cursor) {
    const irr::core::recti dims = mVidDriver->getViewPort();
    irr::core::vector2di centreScreen = irr::core::vector2di(dims.getWidth() / 2, dims.getHeight() / 2);
    cursor->setPosition(centreScreen);
}


void toggleCameraFocus() {
    mCamerasHaveFocus = !mCamerasHaveFocus;

    irr::gui::ICursorControl* cursor = mDevice->getCursorControl();
    cursor->setVisible(!mCamerasHaveFocus);

    if(!mCamerasHaveFocus)
        resetCursorToCenterScr(cursor);
}

void handleInput(double frameTime_s) {
    irr::gui::ICursorControl* cursor = mDevice->getCursorControl();

    toggleCameraFocus();

    if(mHWInput.getMouse().isButtonPressed(Input::MouseState::left) && !ImGui::GetIO().WantCaptureMouse)
        toggleCameraFocus();
}

void update(double frameTime_s) {
    mCamera->setAspectRatio(getAspectRatio());
}


void render() {
    mVidDriver->beginScene(true, true, irr::video::SColor(255, 128, 128, 128));

    chrono::irrlicht::ChIrrTools::drawGrid(
        mVidDriver,
        1,
        1,
        50,
        50,
        chrono::ChCoordsys<>(-mCameraPos, Q_from_AngAxis(chrono::CH_C_PI / 2, chrono::VECT_X)),
        irr::video::SColor(10, 100, 100, 100),
        true);

    mSceneMgr->drawAll();

    ImGui::Begin("This is a test window");
    ImGui::Text("This is some example text");
    ImGui::End();
    ImGui::ShowDemoWindow();
    mImGuiHandle->drawAll();

    mVidDriver->endScene();
}


void run() {
    unsigned lastTime = mDevice->getTimer()->getRealTime();

    while(mDevice->run()) {
        const unsigned now = mDevice->getTimer()->getRealTime();
        const float frameTime_s = static_cast<float>(now - lastTime) / 1000.0f;

        mImGuiHandle->startGUI();

        if(mDevice->isWindowFocused())
            handleInput(frameTime_s);

        update(frameTime_s);
        render();

        lastTime = now;
    }
}


void close() {
    mDevice->closeDevice();
    mDevice->drop();
}


int main() {
    init();
    run();

    return 0;
}
