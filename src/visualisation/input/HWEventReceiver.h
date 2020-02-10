#ifndef INPUT_HWEVENTRECEIVER_H_
#define INPUT_HWEVENTRECEIVER_H_

#include <IEventReceiver.h>
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
        static irr::core::vector2di sPosition_scr;
        static irr::core::vector2di sLastPosition_scr;
        static irr::core::vector2di sDelta_scr;
        static bool sButtonsPressed[3];
        static float sWheelScroll;

    public:
        MouseState() = delete;
        ~MouseState() = default;

        static void init();
        static void update();
        static const irr::core::vector2di& getPosition_scr();
        static const irr::core::vector2di& getDelta_scr();
        static bool isButtonPressed(Button b);
        static float getScroll();
        static void setPosition(irr::core::vector2di newPosition_scr);
    };

    class HWEventReceiver : public irr::IEventReceiver {
    private:
        static bool sKeysDown[irr::KEY_KEY_CODES_COUNT];
        static bool sKeysClicked[irr::KEY_KEY_CODES_COUNT];

    public:
        HWEventReceiver();
        ~HWEventReceiver() = default;

        static void init();
        static void update();
        virtual bool OnEvent(const irr::SEvent& event);
        static bool isKeyPressed(irr::EKEY_CODE key);
        static bool isKeyReleased(irr::EKEY_CODE key);
    };

} // namespace Input

#endif // INPUT_HWEVENTRECEIVER_H_

