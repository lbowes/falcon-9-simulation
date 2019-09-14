#include "Falcon9.h"

namespace Physics {
	namespace Hardware {

		Falcon9::Falcon9(chrono::ChSystemNSC& systemHandle) {
            assemble(systemHandle);
        }

        void Falcon9::update(double dt) {
            // todo: update any domain logic entities for the falcon 9 (as opposed to physics objects that are automatically updated by the system)
            // ...
        }

        void Falcon9::outputToCSV(std::string& destRowCSV) const {
            mS1RP1Tank->outputToCSV(destRowCSV);
            mLandingLeg->outputToCSV(destRowCSV);
            //mSomethingElse->outputToCSV(destRowCSV)
        }

		void Falcon9::assemble(chrono::ChSystemNSC& systemHandle) {
            mS1RP1Tank = std::make_unique<CylinderFluidTank>(
                systemHandle,
			    Propellants::mFluids[Propellants::RP1],
			    14.0,
			    1.83,
			    0.012,
			    2250.0
			);

            mLandingLeg = std::make_unique<LandingLeg>(systemHandle);
        }

	}
}