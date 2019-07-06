#include "HWEventReceiver.h"

namespace Input {

	MouseState::MouseState() :
		mPosition_scr(0, 0),
		mWheelScroll(0.0f)
	{ 
		mButtonsPressed[left] = mButtonsPressed[middle] = mButtonsPressed[right] = false;
	}
	
	HWEventReceiver::HWEventReceiver() {
		for(unsigned key = 0; key < irr::KEY_KEY_CODES_COUNT; key++)
			mKeysDown[key] = mKeysClicked[key] = false;
	}
	
	void HWEventReceiver::update(irr::core::vector2di centreScreenPos_scr) {
		mCentreScreenPos_scr = centreScreenPos_scr;
		mMouseState.mPosition_scr = centreScreenPos_scr;
		mMouseState.mDelta_scr.X = mMouseState.mDelta_scr.Y = 0;
		
		mMouseState.mWheelScroll = 0.0f;
	
		for(bool& k : mKeysClicked)
			k = false;
	}
	
	bool HWEventReceiver::OnEvent(const irr::SEvent& e) {
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
	
	bool HWEventReceiver::isKeyPressed(irr::EKEY_CODE key) const {
		return mKeysDown[key];
	}
	
	bool HWEventReceiver::isKeyReleased(irr::EKEY_CODE key) const {
		return mKeysClicked[key];
	}

}