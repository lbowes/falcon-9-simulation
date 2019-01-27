#include "SupplyLine.h"
#include "CylinderFluidTank.h"

namespace Physics {
	namespace Hardware {

		GasSupplyLine::GasSupplyLine(CylinderFluidTank& gasSource) :
			mGasSource(gasSource)
		{ }

		double GasSupplyLine::transferGas(double massDemand) {
			double gasSupply = mGasSource.getFluidMassValue_tank();
			mGasSource.removeFluid(massDemand);

			return massDemand <= gasSupply ? massDemand : gasSupply;
		}

		BipropSupplyLine::BipropSupplyLine(CylinderFluidTank& oxidiserSource, CylinderFluidTank& fuelSource) :
			mOxidiserSource(oxidiserSource),
			mFuelSource(fuelSource)
		{ }

		double BipropSupplyLine::transferOxidiser(double massDemand) {
			double oxidiserSupply = mOxidiserSource.getFluidMassValue_tank();
			mOxidiserSource.removeFluid(massDemand);
			
			return massDemand <= oxidiserSupply ? massDemand : oxidiserSupply;
		}

		double BipropSupplyLine::transferFuel(double massDemand) {
			double fuelSupply = mFuelSource.getFluidMassValue_tank();
			mFuelSource.removeFluid(massDemand);

			return massDemand <= fuelSupply ? massDemand : fuelSupply;
		}

	}
}