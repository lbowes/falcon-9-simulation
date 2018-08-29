#ifndef PHYSICS_HARDWARE_MERLIN1DVAC_H
#define PHYSICS_HARDWARE_MERLIN1DVAC_H
#pragma once

#include "Physics/Hardware/Common/Propulsion/Engine.h"

namespace Physics {
	namespace Hardware {
		
		class Merlin1DVac : public Engine {
		public:
			Merlin1DVac(GF::CoordTransform3D merlinToStage);
			~Merlin1DVac() = default;

		private:
			void setTypeSpecificParams();

		};

	}
}

#endif
