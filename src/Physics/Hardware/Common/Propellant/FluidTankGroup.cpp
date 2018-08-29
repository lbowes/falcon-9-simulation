#include "FluidTankGroup.h"

namespace Physics {
	namespace Hardware {

		void FluidTankGroup::update() {
			if (mComponents.empty()) return;
			
			updatePropellantTanks();
			updateTotalMass_stage();
			updateTotalCmInertia_stage();
		}

		PropSupplyLine& FluidTankGroup::addPropSupplyLine(FluidTank& oxidiserSource, FluidTank& fuelSource) {
			mPropSupplyLines.push_back(PropSupplyLine(oxidiserSource, fuelSource));
			return mPropSupplyLines.back();
		}

		GasSupplyLine& FluidTankGroup::addGasSupplyLine(FluidTank& gasSource) {
			mGasSupplyLines.push_back(GasSupplyLine(gasSource));
			return mGasSupplyLines.back();
		}

		void FluidTankGroup::updatePropellantTanks() const {
			for (const auto& c : mComponents)
				static_cast<FluidTank*>(c.get())->updateComponentState();
		}

	}
}