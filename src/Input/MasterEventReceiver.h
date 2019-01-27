#ifndef INPUT_MASTEREVENTRECEIVER_H
#define INPUT_MASTEREVENTRECEIVER_H
#pragma once

#include "HWEventReceiver.h"
//TODO
//#include "[header that contains CIMGUIEventReceiver]"
//https://github.com/ZahlGraf/IrrIMGUI#HowToStart

#include <IEventReceiver.h>
#include <irrArray.h>

namespace Input {

	/*
	- This class should be the one passed to the device when the application is initialised, in Visualisation::Visualisation()
	- It should handle events from two other 'sub' event managers, in a chain-of-responsibility method, allowing input events
	  to be processed from hardware (key/button presses, mouse movements etc) as well as GUI elements with IrrIMGUI 
	  eventually.
	- http://irrlicht.sourceforge.net/forum//viewtopic.php?t=13568
	*/
	class MasterEventReceiver : public irr::IEventReceiver {
	private:
		irr::core::array<irr::IEventReceiver*> mReceivers;

	public:
		MasterEventReceiver() = default;
		~MasterEventReceiver() = default;

		void addReciever(irr::IEventReceiver* receiver);
		void removeReceiver(irr::IEventReceiver* receiver);
		
		virtual bool OnEvent(const irr::SEvent& e);

	};

}

#endif
