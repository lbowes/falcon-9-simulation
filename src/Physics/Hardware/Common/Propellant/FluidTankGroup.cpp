#include "FluidTankGroup.h"

namespace Physics {
	namespace Hardware {

		void FluidTankGroup::update() {
			if (mComponents.empty()) 
				return;
			
			updatePropellantTanks();
			updateTotalMass_stage();
			updateTotalCoMInertia_stage();
		}

		void FluidTankGroup::addToTank(unsigned char tankIdx, double mass) {
			static_cast<FluidTank*>(mComponents[tankIdx].get())->addFluid(mass);
			update();
		}

		void FluidTankGroup::emptyTank(unsigned char tankIdx) {
			static_cast<FluidTank*>(mComponents[tankIdx].get())->removeAllFluid();
			update();
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