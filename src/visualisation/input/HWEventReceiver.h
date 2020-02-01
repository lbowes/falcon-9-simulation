#ifndef INPUT_HWEVENTRECEIVER_H_
#define INPUT_HWEVENTRECEIVER_H

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

        const irr::core::vector2di& getPosition_scr() const { return mPosition_scr; }
        const irr::core::vector2di& getDelta_scr() const { return mDelta_scr; }
        bool isButtonPressed(Button b) const { return mButtonsPressed[b]; }
        float getScroll() const { return mWheelScroll; }
        void setPosition(irr::core::vector2di newPosition_scr) { mPosition_scr = newPosition_scr; }
    };

    class HWEventReceiver : public irr::IEventReceiver {
    private:
        MouseState mMouseState;
        bool mKeysDown[irr::KEY_KEY_CODES_COUNT];
        bool mKeysClicked[irr::KEY_KEY_CODES_COUNT];
        irr::core::vector2df mMousePosition;
        irr::core::vector2di mCentreScreenPos_scr;

    public:
        HWEventReceiver();
        ~HWEventReceiver() = default;

        void update(irr::core::vector2di centreScreenPos_scr);
        virtual bool OnEvent(const irr::SEvent& event);
        bool isKeyPressed(irr::EKEY_CODE key) const;
        bool isKeyReleased(irr::EKEY_CODE key) const;
        const MouseState& getMouse() const { return mMouseState; }
        MouseState& getMouse() { return mMouseState; }
    };

} // namespace Input

#endif // INPUT_HWEVENTRECEIVER_H__

