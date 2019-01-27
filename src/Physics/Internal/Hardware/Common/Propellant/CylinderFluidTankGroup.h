#ifndef PHYSICS_HARDWARE_CYLINDERFLUIDTANKGROUP_H
#define PHYSICS_HARDWARE_CYLINDERFLUIDTANKGROUP_H
#pragma once

#include "../Structural/StageComponentGroup.hpp"
#include "SupplyLine.h"

#include <vector>

namespace Physics {
	namespace Hardware {

		class CylinderFluidTank;

		class CylinderFluidTankGroup : public StageComponentGroup {
		private:
			std::vector<BipropSupplyLine> mPropSupplyLines;
			std::vector<GasSupplyLine> mGasSupplyLines;

		public:
			CylinderFluidTankGroup() = default;
			~CylinderFluidTankGroup() = default;

			void addToTank(unsigned char tankIdx, double mass);
			void emptyTank(unsigned char tankIdx);
			BipropSupplyLine& addPropSupplyLine(CylinderFluidTank& oxygenSource, CylinderFluidTank& fuelSource);
			GasSupplyLine& addGasSupplyLine(CylinderFluidTank& gasSource);

		};
	}

}

#endif
