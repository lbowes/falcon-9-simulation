#include "Falcon9.h"
#include <physics/ChSystemNSC.h>

namespace Physics {
	namespace Hardware {

		Falcon9::Falcon9(chrono::ChSystemNSC& sys) :
			ILaunchVehicle(sys),
			mStage1(sys),
			mStage2(sys)
		{
			assemble();
		}

		void Falcon9::assemble() {
			addStage(mStage1);
			addStage(mStage2, chrono::ChFrame<>());
		}

	}
}