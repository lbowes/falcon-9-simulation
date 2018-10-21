#include "SupplyLine.h"

namespace Physics {
	namespace Hardware {

		GasSupplyLine::GasSupplyLine(FluidTank& gasSource) :
			mGasSource(gasSource)
		{ }

		double GasSupplyLine::transferGas(double massDemand) {
			double gasSupply = mGasSource.getFluidMassValue_tank();
			mGasSource.removeFluid(massDemand);

			return massDemand <= gasSupply ? massDemand : gasSupply;
		}

		PropSupplyLine::PropSupplyLine(FluidTank& oxidiserSource, FluidTank& fuelSource) :
			mOxidiserSource(oxidiserSource),
			mFuelSource(fuelSource)
		{ }

		double PropSupplyLine::transferOxidiser(double massDemand) {
			double oxidiserSupply = mOxidiserSource.getFluidMassValue_tank();
			mOxidiserSource.removeFluid(massDemand);
			
			return massDemand <= oxidiserSupply ? massDemand : oxidiserSupply;
		}

		double PropSupplyLine::transferFuel(double massDemand) {
			double fuelSupply = mFuelSource.getFluidMassValue_tank();
			mFuelSource.removeFluid(massDemand);

			return massDemand <= fuelSupply ? massDemand : fuelSupply;
		}

	}
}