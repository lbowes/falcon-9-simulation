#ifndef PHYSICS_HARDWARE_FALCON9_H
#define PHYSICS_HARDWARE_FALCON9_H
#pragma once

#include "../Common/Structural/ILaunchVehicle.h"
#include "Stage_1/Falcon9Stage1.h"
#include "Stage_2/Falcon9Stage2.h"

namespace chrono {
	class ChSystemNSC;
}

namespace Physics {
	namespace Hardware {

		class Falcon9 : public ILaunchVehicle {
		private:
			Falcon9Stage1 mStage1;
			Falcon9Stage2 mStage2;

		public:
			Falcon9(chrono::ChSystemNSC& sys);
			~Falcon9() = default;

			void assemble() override;

		};

	}
}

#endif
