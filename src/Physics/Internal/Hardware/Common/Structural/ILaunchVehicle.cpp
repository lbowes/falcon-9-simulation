#include "ILaunchVehicle.h"
#include "IStage.h"

namespace Physics {
	namespace Hardware {
		
		ILaunchVehicle::ILaunchVehicle(chrono::ChSystemNSC& sys) :
			mSystemHandle(sys)
		{ }
		
		void ILaunchVehicle::update(double dt) {
			for(IStage* s : mStages)
				s->update(dt);
		}

		void ILaunchVehicle::addStage(IStage& s, const chrono::ChFrame<>& LV_to_Stage) {
			mStages.push_back(&s);
		}

	}
}