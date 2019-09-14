// These classes are the intermediaries between the Engines/GasThrusters and their propellant tanks. The aim is to keep these classes separate.
// This class manages the communication between the two. 

// In the future, it would be possible to place ButterflyValve objects in the PropellantLines to adjust propellant mass flow rate.
#ifndef F9SIM_PHYSICS_HARDWARE_SUPPLYLINE_H
#define F9SIM_PHYSICS_HARDWARE_SUPPLYLINE_H
#pragma once

namespace Physics {
	namespace Hardware {
		
		class CylinderFluidTank;

		class GasSupplyLine {
		private:
			CylinderFluidTank& mGasSource;

		public:
			GasSupplyLine(CylinderFluidTank& gasSource);
			~GasSupplyLine() = default;

			double transferGas(double massDemand);

		};

		class BipropSupplyLine {
		private:
			CylinderFluidTank
				&mOxidiserSource,
				&mFuelSource;

		public:
			BipropSupplyLine(CylinderFluidTank& oxidiserSource, CylinderFluidTank& fuelSource);
			~BipropSupplyLine() = default;

			double transferOxidiser(double massDemand);
			double transferFuel(double massDemand);

		};

	}
}

#endif // F9SIM_PHYSICS_HARDWARE_SUPPLYLINE_H
