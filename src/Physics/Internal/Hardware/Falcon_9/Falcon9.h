#ifndef PHYSICS_HARDWARE_FALCON9_H
#define PHYSICS_HARDWARE_FALCON9_H
#pragma once

#include "LandingLeg.h"
#include "../Common/Propellant/CylinderFluidTank.h"

namespace chrono {
	class ChSystemNSC;
}

namespace Physics {
	namespace Hardware {

		class Falcon9 {
		private:
            std::unique_ptr<CylinderFluidTank> mS1RP1Tank;
            std::unique_ptr<LandingLeg> mLandingLeg;

		public:
			Falcon9(chrono::ChSystemNSC& systemHandle);
			~Falcon9() = default;

            void update(double dt);

        private:
			void assemble(chrono::ChSystemNSC& systemHandle);

		};

	}
}

#endif // PHYSICS_HARDWARE_F9_H
