#ifndef PHYSICS_HARDWARE_FALCON9STAGE2_H
#define PHYSICS_HARDWARE_FALCON9STAGE2_H
#pragma once

#include "../../Common/Structural/IStage.h"

namespace chrono {
	class ChSystemNSC;
}

namespace Physics {
	namespace Hardware {

		class Falcon9Stage2 : public IStage {
		private:

		public:
			Falcon9Stage2(chrono::ChSystemNSC& sys);
			~Falcon9Stage2() = default;

			void stageSpecificUpdates(double dt) override;

		};

	}
}

#endif
