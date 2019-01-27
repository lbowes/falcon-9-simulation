#include "F9S1.h"
#include "../../Common/Propellant/Fluid.h"
#include "../../Common/Propellant/CylinderFluidTank.h"

#include <core/ChFrame.h>

//temp
#include <collision/ChCCollisionModel.h>
#include <chrono/physics/ChSystemNSC.h>
//

namespace Physics {
	namespace Hardware {

		F9S1::F9S1(chrono::ChSystemNSC& sys) :
			IStage(sys)
		{
			mBody->SetMass(100000.0);
			mBody->SetFrame_COG_to_REF({0, 20, 0});

			mBody->GetCollisionModel()->ClearModel();
			mBody->GetCollisionModel()->AddCapsule(1.83, 47.0);
			mBody->GetCollisionModel()->BuildModel();

			mBody->SetCollide(true);

			//std::shared_ptr<chrono::collision::ChCollisionModel>();
			//mBody->SetCollisionModel();
			mBody->SetPos({0, 60.0, 0});
			//mBody->SetBodyFixed(true);

			chrono::Quaternion rot = chrono::Q_from_Euler123({0 * chrono::CH_C_DEG_TO_RAD, 0, 0}).GetNormalized();
			mBody->SetRot(rot);
			
			chrono::Quaternion spin = chrono::Q_from_Euler123({0, 180 * chrono::CH_C_DEG_TO_RAD, 0}).GetNormalized();
			mBody->SetRot_dt(spin);

			mSystemHandle.AddBody(mBody);

			addTanks();
		}

		void F9S1::stageSpecificUpdates(double dt) {
			// TODO
			// Update grid fins
			// Update grid fins
		}

		void F9S1::addTanks() {
			const double
				tankWallDensity = 2550.0, // 2550.0
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

	F9S1_DSS::F9S1_DSS(const Hardware::F9S1& f9s1) {
		mS1ToWorldTransform = f9s1.mBody->GetFrame_REF_to_abs();
	}

	void F9S1_DSS::operator=(const F9S1_DSS& other) {
		this->mS1ToWorldTransform = other.mS1ToWorldTransform;
	}

}