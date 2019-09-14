#include "PropellantSubsystem.h"

namespace Physics {
	namespace Hardware {

		void PropellantSubsystem::addToTank(unsigned char tankIdx, double mass) {
			if(tankIdx < mTanks.size())
				mTanks[tankIdx]->addFluid(mass);
			else
				printf("Error: tried adding mass to non-existent tank at index %i\n", tankIdx);
		}

		void PropellantSubsystem::emptyTank(unsigned char tankIdx) {
			if(tankIdx < mTanks.size())
				mTanks[tankIdx]->removeAllFluid();
			else
				printf("Error: emptying non-existent tank at index %i\n", tankIdx);
		}

		BipropSupplyLine& PropellantSubsystem::addPropSupplyLine(CylinderFluidTank& oxidiserSource, CylinderFluidTank& fuelSource) {
			mBipropSupplyLines.push_back(BipropSupplyLine(oxidiserSource, fuelSource));
			return mBipropSupplyLines.back();
		}

		GasSupplyLine& PropellantSubsystem::addGasSupplyLine(CylinderFluidTank& gasSource) {
			mGasSupplyLines.push_back(GasSupplyLine(gasSource));
			return mGasSupplyLines.back();
		}

	}
}