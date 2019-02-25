#ifndef PHYSICS_HARDWARE_PROPELLANTSUBSYSTEM_H
#define PHYSICS_HARDWARE_PROPELLANTSUBSYSTEM_H
#pragma once

#include "SupplyLine.h"
#include "CylinderFluidTank.h"

#include <vector>
#include <memory>

namespace Physics {
	namespace Hardware {

		class PropellantSubsystem {
		private:
			std::vector<BipropSupplyLine> mBipropSupplyLines;
			std::vector<GasSupplyLine> mGasSupplyLines;
			std::vector<std::unique_ptr<CylinderFluidTank>> mTanks;

		public:
			PropellantSubsystem() = default;
			~PropellantSubsystem() = default;

			void addTank(std::unique_ptr<CylinderFluidTank> tank) {
				mTanks.push_back(std::move(tank));
			}

			void addToTank(unsigned char tankIdx, double mass);
			void emptyTank(unsigned char tankIdx);
			BipropSupplyLine& addPropSupplyLine(CylinderFluidTank& oxygenSource, CylinderFluidTank& fuelSource);
			GasSupplyLine& addGasSupplyLine(CylinderFluidTank& gasSource);

		};

	}
}

#endif
