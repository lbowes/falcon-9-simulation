//This class is the intermediary between the Engines and the propellant tanks. The aim is to keep these
//two classes separate, so by adding the propellant between them, they do not have to know
//about each other. This class manages the communication between the two. 

//In the future, it may be possible to place ButterflyValve objects in the PropellantLines to adjust 
//the mass flow rate. Just an idea.
#ifndef PHYSICS_HARDWARE_SUPPLYLINE_H
#define PHYSICS_HARDWARE_SUPPLYLINE_H
#pragma once

#include "FluidTank.h"

namespace Physics {
	namespace Hardware {
		
		class GasSupplyLine {
		private:
			FluidTank& mGasSource;

		public:
			GasSupplyLine(FluidTank& gasSource);
			~GasSupplyLine() = default;

			double transferGas(double massDemand);

		};

		class PropSupplyLine {
		private:
			FluidTank
				&mOxidiserSource,
				&mFuelSource;

		public:
			PropSupplyLine(FluidTank& oxidiserSource, FluidTank& fuelSource);
			~PropSupplyLine() = default;

			double transferOxidiser(double massDemand);
			double transferFuel(double massDemand);

		};

	}
}

#endif
