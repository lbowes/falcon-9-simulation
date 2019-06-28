#include "CylinderFluidTank.h"

#include <chrono/utils/ChCompositeInertia.h>
#include <chrono/utils/ChUtilsGeometry.h>
#include <chrono/physics/ChSystemNSC.h>
#include <iomanip>

namespace Physics {
	namespace Hardware {

		CylinderFluidTank::CylinderFluidTank(chrono::ChSystemNSC& system, Fluid f, double height, double radius, double thickness, double density) :
			mFluid(f),
			mHeight(height),
			mRadius(radius),
			mWallThickness(thickness),
			mMaterialDensity(density),
			mInternalHeight(mHeight - 2.0 * mWallThickness),
			mVolume_internal(chrono::utils::CalcCylinderVolume(radius - thickness, mInternalHeight * 0.5)),
			mMaxFluidMass(mFluid.mDensity * mVolume_internal),
			mTankMass(mMaterialDensity * (chrono::utils::CalcCylinderVolume(radius, mHeight * 0.5) - mVolume_internal)),
			mTankCoM_tank({0, height * 0.5, 0}),
			mTankInertia_tank(tankInertia_tank())
		{ 
            assemble(system);
		}

		void CylinderFluidTank::addFluid(double mass) {
			mFluidMass = std::min(mFluidMass + mass, mMaxFluidMass);
			onFluidMassChange();
		}

		void CylinderFluidTank::removeFluid(double mass) {
			mFluidMass = std::max(mFluidMass - mass, 0.0);
			onFluidMassChange();
		}

		void CylinderFluidTank::removeAllFluid() {
			mFluidMass = 0.0;
			onFluidMassChange();
		}

        void CylinderFluidTank::outputToCSV(std::string& destRowCSV) const {   
            std::stringstream ss;
            ss << std::setprecision(std::numeric_limits<long double>::digits10 + 1);

            // Position            
            const chrono::ChVector<> pos = mBody->GetFrame_REF_to_abs().GetPos();
            ss << pos.x() << "," << pos.y() << "," << pos.z() << ",";
            
            // Rotation
            const chrono::ChQuaternion<> rot = mBody->GetFrame_REF_to_abs().GetRot();
            ss << rot.e0() << "," << rot.e1() << "," << rot.e2() << "," << rot.e3() << ",";
            
            destRowCSV += ss.str();
        }

		void CylinderFluidTank::assemble(chrono::ChSystemNSC& system) {
            mBody = std::make_shared<chrono::ChBodyAuxRef>();
            system.AddBody(mBody);
            
            // Mass/inertia properties
			mBody->SetMass(combinedMass());
			mBody->SetInertia(combinedInertia_tank());

            // Material properties
			mBody->GetMaterialSurfaceNSC()->SetFriction(1);
			mBody->GetMaterialSurfaceNSC()->SetSpinningFriction(1);

			// Collision properties
			mBody->GetCollisionModel()->SetEnvelope(2.0);
			mBody->GetCollisionModel()->SetSafeMargin(2.0);
			mBody->GetCollisionModel()->ClearModel();
			mBody->GetCollisionModel()->AddCylinder(mRadius, mRadius, mHeight * 0.5, mTankCoM_tank);
			mBody->GetCollisionModel()->BuildModel();
			mBody->SetCollide(true);

            // Centre of mass
            mBody->SetFrame_COG_to_REF(combinedCoM_tank());

			// temp testing
            mBody->SetPos({0, 10.0, 0});
            //mBody->SetRot(chrono::Q_from_AngX(0.3));
            //mBody->SetRot_dt(chrono::Q_from_AngX(3.14));
            //

            // ------------- CURRENTLY WORKING ON ------------- 
            // - Making sure that all physics above is correct for the tank now that the visualisation is working (test when full)
            // - Look into visualising coordinate frames (origin, CoM) of tank
            // - Generalise the current solution to work with other components once the tank has been implemented correctly
		}

		// void CylinderFluidTank::attachToStage() {
        //     // Move the tank into the correct position/orientation on the stage   
        //     mBody->SetFrame_REF_to_abs(mTransform_stage >> mStageBodyHandle->GetFrame_REF_to_abs());
            
        //     mStageLink1 = std::make_shared<chrono::ChLinkLockLock>();
    	// 	mStageLink1->Initialize(mStageBodyHandle, mBody, chrono::ChCoordsys(chrono::Vector(0, 0, 0)));
		// 	mStageBodyHandle->GetSystem()->AddLink(mStageLink1);

        //     mStageLink2 = std::make_shared<chrono::ChLinkLockLock>();
    	// 	mStageLink2->Initialize(mStageBodyHandle, mBody, chrono::ChCoordsys(chrono::Vector(0, 30, 0)));
		// 	mStageBodyHandle->GetSystem()->AddLink(mStageLink2);
		// }

		double CylinderFluidTank::combinedMass() const {
			return mTankMass + mFluidMass;
		}

		chrono::ChFrame<> CylinderFluidTank::combinedCoM_tank() const {
			const chrono::Vector CoM_tank = (mTankMass * mTankCoM_tank + mFluidMass * mFluidCoM_tank) / (mTankMass + mFluidMass);
			return chrono::ChFrame(CoM_tank);
		}

		chrono::ChMatrix33<> CylinderFluidTank::combinedInertia_tank() const {
			return mTankInertia_tank + mFluidInertia_tank;
		}

		chrono::ChMatrix33<> CylinderFluidTank::tankInertia_tank() const {
			using namespace chrono;

			// Define the coordinate frame of the tank's CoM with respect to its origin
			const ChFrame<> mTankCoM = ChFrame<>(mTankCoM_tank);
			utils::CompositeInertia tankInertia_tank;

			// Calculate the (local) inertia of a solid cylinder with the outer dimensions of the tank, around it's CoM
			ChMatrix33<> tankInertiaSolid_tankCoM = utils::CalcCylinderGyration(mRadius, mHeight * 0.5);
			
			// Place this inertia at the tank's CoM (relative to the tank's origin)
			// Internally, AddComponent() uses the parallel axis theorem
			tankInertia_tank.AddComponent(mTankCoM, mTankMass, tankInertiaSolid_tankCoM);

			// Calculate the (local) inertia of a void cylinder with the inner dimensions of the tank, around it's CoM
			// This will 'hollow out' the tank
			ChMatrix33<> tankInertiaVoid_tankCoM = utils::CalcCylinderGyration(mRadius - mWallThickness, mHeight * 0.5 - mWallThickness);
			
			// Place this inertia at the tank's CoM (relative to the tank's origin) and make it a void material
			const double voidMass = mVolume_internal * mMaterialDensity;
			tankInertia_tank.AddComponent(mTankCoM, voidMass, tankInertiaVoid_tankCoM, true);

			// The final result should contain the inertia of a thick-walled but hollow cylinder, about the origin of the tank (ie. base of cylinder)
			// For a stage component, this is the correct form and space for the inertia to be in
			return tankInertia_tank.GetInertia() * mTankMass;
		}

		void CylinderFluidTank::onFluidMassChange() 
			// Responsible for updating various tank properties after the fluid mass has changed
		{
			mFluidVolume = mFluidMass / mFluid.mDensity;
			mFluidLevel = (mFluidVolume / mVolume_internal) * mInternalHeight;
			mPercentFull = mFluidMass / mMaxFluidMass;

			// addFluid() and removeFluid() handle the change of value of the fluid mass,
			// but it is up to this function to then update the centre of mass after any change
			if (mFluid.mState == FluidState::liquid)
				mFluidCoM_tank = { 0, mWallThickness + mFluidLevel * 0.5, 0 };
			else if(mFluid.mState == FluidState::gas)
				mFluidCoM_tank = { 0, mWallThickness + mInternalHeight * 0.5, 0 };

			// After the fluid level has changed, its inertia must be updated (in tank space) 
			mFluidInertia_tank = chrono::utils::CalcCylinderGyration(mRadius - mWallThickness, mHeight * 0.5 - mWallThickness, mFluidCoM_tank);
			
			// Update mBody properties
			mBody->SetMass(combinedMass());
			mBody->SetInertia(combinedInertia_tank());
			mBody->SetFrame_COG_to_REF(combinedCoM_tank());
		}

	}
}