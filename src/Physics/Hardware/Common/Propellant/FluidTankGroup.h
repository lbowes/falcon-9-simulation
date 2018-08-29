#ifndef FLUIDTANKGROUP_H
#define FLUIDTANKGROUP_H
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
			PropSupplyLine& addPropSupplyLine(FluidTank& oxidiserSource, FluidTank& fuelSource);
			GasSupplyLine& addGasSupplyLine(FluidTank& gasSource);

		private:
			void updatePropellantTanks() const;

		};
	}

}

#endif
