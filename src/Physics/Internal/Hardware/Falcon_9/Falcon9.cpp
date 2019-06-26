#include "Falcon9.h"

namespace Physics {
	namespace Hardware {

		Falcon9::Falcon9(chrono::ChSystemNSC& systemHandle) {
            assemble(systemHandle);
        }

        void Falcon9::update(double dt) {
            // ...
        }

		void Falcon9::assemble(chrono::ChSystemNSC& systemHandle) {
            //mS1RP1Tank = std::make_unique<CylinderFluidTank>(
            //    systemHandle,
			//    Propellants::mFluids[Propellants::RP1],
			//    14,
			//    1.83,
			//    0.012,
			//    2250 
			//);
        }

	}
}