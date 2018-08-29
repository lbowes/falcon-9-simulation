#ifndef PHYSICS_HARDWARE_MERLIN1D_H
#define PHYSICS_HARDWARE_MERLIN1D_H
#pragma once

#include "Physics/Hardware/Common/Propulsion/Engine.h"

namespace Physics {
	namespace Hardware {
		
		class Merlin1D : public Engine {
		public:
			Merlin1D(GF::CoordTransform3D merlinToStage);
			~Merlin1D() = default;

		private:
			void setTypeSpecificParams();

		};

	}
}

#endif
