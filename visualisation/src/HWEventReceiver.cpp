#include "HWEventReceiver.h"


namespace Input {

    irr::core::vector2di MouseState::sPosition_scr;
    irr::core::vector2di MouseState::sLastPosition_scr;
    irr::core::vector2di MouseState::sDelta_scr;
    float MouseState::sWheelScroll;
    bool MouseState::sButtonsPressed[3];

    bool HWEventReceiver::sKeysDown[irr::KEY_KEY_CODES_COUNT];
    bool HWEventReceiver::sKeysClicked[irr::KEY_KEY_CODES_COUNT];


    void MouseState::init() {
        sButtonsPressed[left] = sButtonsPressed[middle] = sButtonsPressed[right] = false;
    }


    void MouseState::update() {
        for(bool& b : sButtonsPressed)
            b = false;

        sDelta_scr = sPosition_scr - sLastPosition_scr;
        sLastPosition_scr = sPosition_scr;
    }


    const irr::core::vector2di& MouseState::getPosition_scr() {
        return sPosition_scr;
    }


    const irr::core::vector2di& MouseState::getDelta_scr() {
        return sDelta_scr;
    }


    bool MouseState::isButtonPressed(Button b) {
        return sButtonsPressed[b];
    }


    float MouseState::getScroll() {
        return sWheelScroll;
    }


    void MouseState::setPosition(irr::core::vector2di newPosition_scr) {
        sPosition_scr = newPosition_scr;
        sLastPosition_scr = newPosition_scr;
    }


    HWEventReceiver::HWEventReceiver() {
        init();
    }


    void HWEventReceiver::init() {
        for(unsigned key = 0; key < irr::KEY_KEY_CODES_COUNT; key++)
            sKeysDown[key] = sKeysClicked[key] = false;
    }


    void HWEventReceiver::update() {
        MouseState::update();

        for(bool& k : sKeysClicked)
            k = false;
    }


    bool HWEventReceiver::OnEvent(const irr::SEvent& e) {
        using namespace irr;

        switch(e.EventType) {
        case EET_KEY_INPUT_EVENT: {
            sKeysDown[e.KeyInput.Key] = e.KeyInput.PressedDown;
            sKeysClicked[e.KeyInput.Key] = !e.KeyInput.PressedDown;
            break;
        }
        case EET_MOUSE_INPUT_EVENT: {
            switch(e.MouseInput.Event) {
            case EMIE_LMOUSE_PRESSED_DOWN: {
                MouseState::sButtonsPressed[MouseState::Button::left] = true;
                break;
            }
            case EMIE_LMOUSE_LEFT_UP: {
                MouseState::sButtonsPressed[MouseState::Button::left] = false;
                break;
            }
            case EMIE_MMOUSE_PRESSED_DOWN: {
                MouseState::sButtonsPressed[MouseState::Button::middle] = true;
                break;
            }
            case EMIE_MMOUSE_LEFT_UP: {
                MouseState::sButtonsPressed[MouseState::Button::middle] = false;
                break;
            }
            case EMIE_RMOUSE_PRESSED_DOWN: {
                MouseState::sButtonsPressed[MouseState::Button::right] = true;
                break;
            }
            case EMIE_RMOUSE_LEFT_UP: {
                MouseState::sButtonsPressed[MouseState::Button::right] = false;
                break;
            }
            case EMIE_MOUSE_MOVED: {
                MouseState::sPosition_scr = irr::core::vector2di(e.MouseInput.X, e.MouseInput.Y);
                break;
            }
            case EMIE_MOUSE_WHEEL: {
                MouseState::sWheelScroll = e.MouseInput.Wheel;
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


    bool HWEventReceiver::isKeyPressed(irr::EKEY_CODE key) {
        return sKeysDown[key];
    }


    bool HWEventReceiver::isKeyReleased(irr::EKEY_CODE key) {
        return sKeysClicked[key];
    }


} // namespace Input
