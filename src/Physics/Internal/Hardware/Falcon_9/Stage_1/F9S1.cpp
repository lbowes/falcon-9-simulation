#include "F9S1.h"
#include "../../Common/Propellant/Fluid.h"
#include "../../Common/Propellant/CylinderFluidTank.h"

#include <chrono/physics/ChSystemNSC.h>
#include <core/ChFrame.h>
#include <core/ChMatrix33.h>
#include <chrono/utils/ChCompositeInertia.h>
#include <chrono/utils/ChUtilsGeometry.h>

namespace Physics {
	namespace Hardware {

		F9S1::F9S1(chrono::ChSystemNSC& sys) :
			IStage(sys)
		{
            // temp: positioning the vehicle in the world (should be done via the launch vehicle eventually)
        	//mBody->SetFrame_REF_to_abs(chrono::ChFrame(chrono::Vector(0, 10, 10), chrono::Q_from_AngX(0.6)));
			//mBody->SetRot(chrono::Q_from_AngY(chrono::CH_C_PI));
			//mBody->SetRot_dt(chrono::Q_from_AngAxis(90.0, {0, 1, 0}));
            mBody->SetWvel_loc(chrono::Vector(0, 100, 0));
            mBody->SetFrame_REF_to_abs(chrono::ChFrame(chrono::Vector(-30, 0, 0)));

        	assemble();
		}

		void F9S1::stageSpecificUpdates(double dt) {
			// TODO: Update entities specific to the Falcon 9 first stage. Not all stage's have landing legs, or grid fins etc,
			// so these should be updated here.
		}

		void F9S1::addMiscMass() {
			// TODO: Once all subcomponents have been added, look at the difference between the total mass so far and the
			// ground truth mass values given online. The stage should reach the correct mass value.
			
			// temp - this is not accurate, just used to allow debugging other features
			mBody->SetMass(20);
            mBody->SetInertia(chrono::utils::CalcCylinderGyration(1.83, 23.5, chrono::Vector(0, 23.5, 0)) * 10);
		}

		void F9S1::addPropellantSupplies() {
			const double
				tankWallDensity = 2550, // 2550
				tankWallThickness = 0.012,
				radius = 1.83;

			mPropSupplies.addTank(std::make_unique<CylinderFluidTank>(
				mBody,
				chrono::ChFrame(chrono::Vector(0, 17, 0)), 
				Propellants::mFluids[Propellants::liquidOxygen],
				22.8, 
				radius, 
				tankWallThickness, 
				tankWallDensity
			));

			mPropSupplies.addTank(std::make_unique<CylinderFluidTank>(
			    mBody,
			    chrono::ChFrame(chrono::Vector(0, 0, 0)), //2.3
			    Propellants::mFluids[Propellants::RP1],
			    14,
			    radius,
			    tankWallThickness,
			    tankWallDensity 
			));
			
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

		void F9S1::addEngines() {
			// TODO: Add 9 Merlin 1Ds to the octaweb	
		}

	}

	F9S1_DSS::F9S1_DSS(const Hardware::F9S1& f9s1) {
		mS1ToWorldTransform = f9s1.mBody->GetFrame_REF_to_abs();
		//mTankToWorldTransform = f9s1.mPropSupplies.getTank(0).getFrame_world();
	}

	void F9S1_DSS::operator=(const F9S1_DSS& other) {
		this->mS1ToWorldTransform = other.mS1ToWorldTransform;
		//this->mTankToWorldTransform = other.mTankToWorldTransform;
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
		//pos_a = a.mTankToWorldTransform.GetPos(),
		//pos_b = b.mTankToWorldTransform.GetPos(),
		//lerpPos = pos_a + (pos_b - pos_a) * x;
//
	 	//result.mTankToWorldTransform.SetPos(lerpPos);
//
		//rot_a = a.mTankToWorldTransform.GetRot(),
		//rot_b = b.mTankToWorldTransform.GetRot(),
		//difference = rot_b - rot_a,
		//lerpRot_world = rot_a + difference * x;
//
		//result.mTankToWorldTransform.SetRot(lerpRot_world);

		return result;
	}

}