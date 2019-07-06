//http://irrlicht.sourceforge.net/forum//viewtopic.php?t=13568

#include "MasterEventReceiver.h"

namespace Input {

	void MasterEventReceiver::addReciever(irr::IEventReceiver* receiver) {
      	irr::s32 r = mReceivers.linear_search(receiver); 
      	if (r != -1) 
         	return;

      	mReceivers.push_back(receiver);
	}

	void MasterEventReceiver::removeReceiver(irr::IEventReceiver* receiver) {
		irr::s32 r = mReceivers.linear_search(receiver); 
     	if (r != -1) 
       		mReceivers.erase(r); 
	}

	bool MasterEventReceiver::OnEvent(const irr::SEvent& e) {
		for (unsigned r = 0; r < mReceivers.size(); r++) { 
        	if (mReceivers[r]->OnEvent(e)) 
           		return true; 
      	} 

      	return false;
	}

}