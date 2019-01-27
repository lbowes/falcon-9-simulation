#ifndef PHYSICS_HARDWARE_FALCON9STAGE1_H
#define PHYSICS_HARDWARE_FALCON9STAGE1_H
#pragma once

#include "../../Common/Structural/IStage.h"

namespace chrono {
	class ChSystemNSC;
}

namespace Physics {
	namespace Hardware {

		class Falcon9Stage1 : public IStage {
		private:

		public:
			Falcon9Stage1(chrono::ChSystemNSC& sys);
			~Falcon9Stage1() = default;

			void stageSpecificUpdates(double dt) override;

		private:
			void addTanks();

		};

	}
}

#endif
