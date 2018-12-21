#ifndef PHYSICS_HARDWARE_MERLIN1D_H
#define PHYSICS_HARDWARE_MERLIN1D_H
#pragma once

#include "Physics/Hardware/Common/Propulsion/Engine.h"

namespace Physics {
	namespace Hardware {
		
		class Merlin1D : public Engine {
		public:
			Merlin1D(CoordTransform3D merlinToStage);
			~Merlin1D() = default;

			void gimbalTo(double clockDirAngle_degs, double angle_degs);

		private:
			void setTypeSpecificParams();

		};

	}
}

#endif
