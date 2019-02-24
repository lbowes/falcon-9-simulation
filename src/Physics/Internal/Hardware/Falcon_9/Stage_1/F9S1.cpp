#include "F9S1.h"
#include "../../Common/Propellant/Fluid.h"
#include "../../Common/Propellant/CylinderFluidTank.h"

#include <chrono/physics/ChSystemNSC.h>
#include <core/ChFrame.h>
#include <core/ChMatrix33.h>

//temp
#include <chrono/utils/ChUtilsGeometry.h>
#include <chrono/collision/ChCCollisionModel.h>
//

namespace Physics {
	namespace Hardware {

		F9S1::F9S1(chrono::ChSystemNSC& sys) :
			IStage(sys)
		{
			assemble();
		}

		void F9S1::stageSpecificUpdates(double dt) {
			//TODO: 
		}

		void F9S1::addPropellantSupplies() {
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

	F9S1_DSS F9S1_DSS::lerp(const F9S1_DSS& a, const F9S1_DSS& b, float x) {
		F9S1_DSS result;
		
		const chrono::Vector 
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

		return result;
	}

}