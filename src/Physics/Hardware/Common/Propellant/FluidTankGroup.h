#ifndef PHYSICS_HARDWARE_FLUIDTANKGROUP_H
#define PHYSICS_HARDWARE_FLUIDTANKGROUP_H
#pragma once

#include "FluidTank.h"
#include "Physics/Hardware/Common/Core/StageComponentGroup.h"
#include "SupplyLine.h"

#include <vector>

namespace Physics {
	namespace Hardware {
		
		class FluidTankGroup : public StageComponentGroup {
		private:
			std::vector<PropSupplyLine> mPropSupplyLines;
			std::vector<GasSupplyLine> mGasSupplyLines;

		public:
			FluidTankGroup() = default;
			~FluidTankGroup() = default;

			void update();
			void addToTank(unsigned char tankIdx, double mass);
			void emptyTank(unsigned char tankIdx);
			PropSupplyLine& addPropSupplyLine(FluidTank& oxidiserSource, FluidTank& fuelSource);
			GasSupplyLine& addGasSupplyLine(FluidTank& gasSource);

		private:
			void updatePropellantTanks() const;

		};
	}

}

#endif
