#include "SupplyLine.h"

namespace Physics {
	namespace Hardware {

		GasSupplyLine::GasSupplyLine(FluidTank& gasSource) :
			mGasSource(gasSource)
		{ }

		double GasSupplyLine::transferGas(double massDemand) {
			double gasSupply = mGasSource.getPropMassValue_tank();
			mGasSource.removePropellant(massDemand);

			return massDemand <= gasSupply ? massDemand : gasSupply;
		}

		PropSupplyLine::PropSupplyLine(FluidTank& oxidiserSource, FluidTank& fuelSource) :
			mOxidiserSource(oxidiserSource),
			mFuelSource(fuelSource)
		{ }

		double PropSupplyLine::transferOxidiser(double massDemand) {
			double oxidiserSupply = mOxidiserSource.getPropMassValue_tank();
			mOxidiserSource.removePropellant(massDemand);
			
			return massDemand <= oxidiserSupply ? massDemand : oxidiserSupply;
		}

		double PropSupplyLine::transferFuel(double massDemand) {
			double fuelSupply = mFuelSource.getPropMassValue_tank();
			mFuelSource.removePropellant(massDemand);

			return massDemand <= fuelSupply ? massDemand : fuelSupply;
		}

	}
}