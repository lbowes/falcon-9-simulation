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
        irr::core::vector2di mPosition_scr;
        irr::core::vector2di mDelta_scr;
        bool mButtonsPressed[3];
        float mWheelScroll;

    public:
        MouseState();
        ~MouseState() = default;

        const irr::core::vector2di& getPosition_scr() const;
        const irr::core::vector2di& getDelta_scr() const;
        bool isButtonPressed(Button b) const;
        float getScroll() const;
        void setPosition(irr::core::vector2di newPosition_scr);
    };

    class HWEventReceiver : public irr::IEventReceiver {
    private:
        static MouseState mMouseState;
        static bool mKeysDown[irr::KEY_KEY_CODES_COUNT];
        static bool mKeysClicked[irr::KEY_KEY_CODES_COUNT];
        static irr::core::vector2di mMousePosition;
        static irr::core::vector2di mCentreScreenPos_scr;

    public:
        HWEventReceiver();
        ~HWEventReceiver() = default;

        static void update(irr::core::vector2di centreScreenPos_scr);
        virtual bool OnEvent(const irr::SEvent& event);
        static bool isKeyPressed(irr::EKEY_CODE key);
        static bool isKeyReleased(irr::EKEY_CODE key);
        static const MouseState& getMouse();
    };

} // namespace Input

#endif // INPUT_HWEVENTRECEIVER_H_

