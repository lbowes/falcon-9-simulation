#include "F9.h"
#include <physics/ChSystemNSC.h>

namespace Physics {
	namespace Hardware {

		F9::F9(chrono::ChSystemNSC& sys) :
			ILaunchVehicle(sys),
			mStage1(sys),
			mStage2(sys)
		{
			assemble();
		}

		void F9::assemble() {
			addStage(mStage1);
			addStage(mStage2, chrono::ChFrame<>());
		}

	}
		
	F9_DSS::F9_DSS(const Hardware::F9& f9) :
		mF9S1_dss(f9.mStage1)
	{ }

	void F9_DSS::operator=(const F9_DSS& other) {
		this->mF9S1_dss = other.mF9S1_dss;
	}

}