#ifndef PHYSICS_HARDWARE_F9S2_H
#define PHYSICS_HARDWARE_F9S2_H
#pragma once

#include "../../Common/Structural/IStage.h"

namespace chrono {
	class ChSystemNSC;
}

namespace Physics {
	namespace Hardware {

		class F9S2 : public IStage {
		private:

		public:
			F9S2(chrono::ChSystemNSC& sys);
			~F9S2() = default;

			void stageSpecificUpdates(double dt) override;

		};

	}
}

#endif
