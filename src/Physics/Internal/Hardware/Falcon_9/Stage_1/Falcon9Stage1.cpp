#include "Falcon9Stage1.h"
#include "../../Common/Propellant/Fluid.h"
#include "../../Common/Propellant/CylinderFluidTank.h"

#include <core/ChFrame.h>

namespace Physics {
	namespace Hardware {

		Falcon9Stage1::Falcon9Stage1(chrono::ChSystemNSC& sys) :
			IStage(sys)
		{
			// mBody->SetMass(1.0);
			// mBody->SetFrame_COG_to_REF({0, 20, 0});
			// mBody->SetPos({0, 100, 0});	

			addTanks();
		}

		void Falcon9Stage1::stageSpecificUpdates(double dt) {
			// TODO
			// Update grid fins
			// Update grid fins
		}

		void Falcon9Stage1::addTanks() {
			const double
				tankWallDensity = 2'550.0, // 2'550.0
				tankWallThickness = 0.012,
				radius = 1.83;

			mPropSupplies.addComponent(std::make_unique<CylinderFluidTank>(
				chrono::ChFrame<>(chrono::ChVector<>(0.0, 16.38, 0.0)), 
				Propellants::mFluids[Propellants::liquidOxygen],
				22.8, 
				radius, 
				tankWallThickness, 
				tankWallDensity
			));
			
			mPropSupplies.addComponent(std::make_unique<CylinderFluidTank>(
				chrono::ChFrame<>(chrono::ChVector<>(0.0, 2.3, 0.0)), 
				Propellants::mFluids[Propellants::RP1],
				14.0, 
				radius,
				tankWallThickness,
				tankWallDensity 
			));
			
			mPropSupplies.addComponent(std::make_unique<CylinderFluidTank>(
				chrono::ChFrame<>(chrono::ChVector<>(0.0, 40.0, 0.0)), 
				Propellants::mFluids[Propellants::nitrogen],
				0.36, 
				0.36, 
				0.042, 
				tankWallDensity
			));
		}

	}
}