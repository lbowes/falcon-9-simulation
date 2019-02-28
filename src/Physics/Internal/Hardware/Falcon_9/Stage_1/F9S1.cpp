#include "F9S1.h"
#include "../../Common/Propellant/Fluid.h"
#include "../../Common/Propellant/CylinderFluidTank.h"

#include <chrono/physics/ChSystemNSC.h>
#include <core/ChFrame.h>
#include <core/ChMatrix33.h>

// temp
#include <chrono/utils/ChUtilsGeometry.h>
//

// TODO: (Add this to the TODO file)
// Look into using the json library to save the state of the simulation (camera position, speed controls, time etc)

namespace Physics {
	namespace Hardware {

		F9S1::F9S1(chrono::ChSystemNSC& sys) :
			IStage(sys)
		{
			assemble();

            // TODO: All stage objects must be given some sort of fundamental mass and inertia.
            // Even though most of the mass will come from other objects being linked bound to the stage body, the
            // simulation does not like it if a very low mass/inertia object is bound to a very high inertia body.
            // Perhaps this is where the 'miscellaneous inert mass' feature could fit in? Remaining mass not accounted
            // for in sub components could be directly given to the stage body.

			// temp - this is just a temporary 10kg cylinder covering the height of the 1st stage while component adding
			// is debugged
			mBody->SetMass(10);
            mBody->SetInertia(chrono::utils::CalcCylinderGyration(1.83, 23.5, chrono::Vector(0, 23.5, 0)) * 10);
            //

			mBody->SetCollide(true);

			// TODO: Look into how the components can be added such that they can never collide with the stage
			// This can be done neatly.
			mBody->GetCollisionModel()->SetFamily(2);
			mBody->GetCollisionModel()->SetFamilyMaskNoCollisionWithFamily(3);
		}

		void F9S1::stageSpecificUpdates(double dt) {
			// TODO:
		}

		void F9S1::addPropellantSupplies() {
			const double
				tankWallDensity = 2550.0, // 2550.0
				tankWallThickness = 0.012,
				radius = 1.83;

			mPropSupplies.addTank(std::make_unique<CylinderFluidTank>(
				mSystemHandle,
				mBody,
				chrono::ChFrame(chrono::Vector(0.0, 16.38, 0.0)), 
				Propellants::mFluids[Propellants::liquidOxygen],
				22.8, 
				radius, 
				tankWallThickness, 
				tankWallDensity
			));

			//mPropSupplies.addTank(std::make_unique<CylinderFluidTank>(
			//	mSystemHandle,
			//	mBody,
			//	chrono::ChFrame(chrono::Vector(0.0, 2.3, 0.0)), 
			//	Propellants::mFluids[Propellants::RP1],
			//	14.0, 
			//	radius,
			//	tankWallThickness,
			//	tankWallDensity 
			//));
			//
			//mPropSupplies.addTank(std::make_unique<CylinderFluidTank>(
			//	mSystemHandle,
			//	mBody,
			//	chrono::ChFrame(chrono::Vector(0.0, 40.0, 0.0)), 
			//	Propellants::mFluids[Propellants::nitrogen],
			//	0.36, 
			//	0.36, 
			//	0.042, 
			//	tankWallDensity
			//));
		}

	}

	F9S1_DSS::F9S1_DSS(const Hardware::F9S1& f9s1) {
		mS1ToWorldTransform = f9s1.mBody->GetFrame_REF_to_abs();
		mTankToWorldTransform = f9s1.mPropSupplies.getTank(0).getFrame_world();
	}

	void F9S1_DSS::operator=(const F9S1_DSS& other) {
		this->mS1ToWorldTransform = other.mS1ToWorldTransform;
		this->mTankToWorldTransform = other.mTankToWorldTransform;
	}

	F9S1_DSS F9S1_DSS::lerp(const F9S1_DSS& a, const F9S1_DSS& b, float x) {
		F9S1_DSS result;

		// mS1ToWorldTransform		
		chrono::Vector 
			pos_a = a.mS1ToWorldTransform.GetPos(),
			pos_b = b.mS1ToWorldTransform.GetPos(),
			lerpPos = pos_a + (pos_b - pos_a) * x;

	 	result.mS1ToWorldTransform.SetPos(lerpPos);

		chrono::Quaternion
			rot_a = a.mS1ToWorldTransform.GetRot(),
			rot_b = b.mS1ToWorldTransform.GetRot(),
			difference = rot_b - rot_a,
			lerpRot_world = rot_a + difference * x;

		result.mS1ToWorldTransform.SetRot(lerpRot_world);

        // temp - just added to debug adding tank components
		// mTankToWorldTransform
		pos_a = a.mTankToWorldTransform.GetPos(),
		pos_b = b.mTankToWorldTransform.GetPos(),
		lerpPos = pos_a + (pos_b - pos_a) * x;

	 	result.mTankToWorldTransform.SetPos(lerpPos);

		rot_a = a.mTankToWorldTransform.GetRot(),
		rot_b = b.mTankToWorldTransform.GetRot(),
		difference = rot_b - rot_a,
		lerpRot_world = rot_a + difference * x;

		result.mTankToWorldTransform.SetRot(lerpRot_world);

		return result;
	}

}