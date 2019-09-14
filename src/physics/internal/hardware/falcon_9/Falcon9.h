#ifndef F9SIM_PHYSICS_HARDWARE_FALCON9_H_
#define F9SIM_PHYSICS_HARDWARE_FALCON9_H_
#pragma once

#include "LandingLeg.h"
#include "../common/propellant/CylinderFluidTank.h"
#include "../../../IDataSource.hpp"

namespace chrono {
	class ChSystemNSC;
}

namespace Physics {
	namespace Hardware {

		class Falcon9 : public IDataSource {
		private:
            std::unique_ptr<CylinderFluidTank> mS1RP1Tank;
            std::unique_ptr<LandingLeg> mLandingLeg;

		public:
			Falcon9(chrono::ChSystemNSC& systemHandle);
			~Falcon9() = default;

            void update(double dt);
            virtual void outputToCSV(std::string& destRowCSV) const override;

        private:
			void assemble(chrono::ChSystemNSC& systemHandle);

		};

	}
}

#endif // F9SIM_PHYSICS_HARDWARE_FALCON9_H_
