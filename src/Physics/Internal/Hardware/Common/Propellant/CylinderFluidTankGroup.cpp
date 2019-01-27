#include "CylinderFluidTankGroup.h"
#include "CylinderFluidTank.h"

namespace Physics {
	namespace Hardware {

		void CylinderFluidTankGroup::addToTank(unsigned char tankIdx, double mass) {
			static_cast<CylinderFluidTank*>(mComponents[tankIdx].get())->addFluid(mass);
		}

		void CylinderFluidTankGroup::emptyTank(unsigned char tankIdx) {
			static_cast<CylinderFluidTank*>(mComponents[tankIdx].get())->removeAllFluid();
		}

		BipropSupplyLine& CylinderFluidTankGroup::addPropSupplyLine(CylinderFluidTank& oxidiserSource, CylinderFluidTank& fuelSource) {
			mPropSupplyLines.push_back(BipropSupplyLine(oxidiserSource, fuelSource));
			return mPropSupplyLines.back();
		}

		GasSupplyLine& CylinderFluidTankGroup::addGasSupplyLine(CylinderFluidTank& gasSource) {
			mGasSupplyLines.push_back(GasSupplyLine(gasSource));
			return mGasSupplyLines.back();
		}

	}
}