#include "CylinderFluidTank.h"

#include <chrono/utils/ChCompositeInertia.h>
#include <chrono/utils/ChUtilsGeometry.h>

namespace Physics {
	namespace Hardware {

		CylinderFluidTank::CylinderFluidTank(chrono::ChSystemNSC& sys, std::shared_ptr<chrono::ChBodyAuxRef>& stageBody, const chrono::ChFrame<>& tank_to_Stage, Fluid f, double height, double radius, double thickness, double density) :
			IStageComponent(sys, stageBody, tank_to_Stage),
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
			assemble();
			attachToStage();
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

		void CylinderFluidTank::assemble() {
			//mBody->SetMass(combinedMass());
			//mBody->SetInertia(combinedInertia_tank());
			mBody->SetCollide(false);
			//mBody->SetFrame_COG_to_REF(combinedCoM_tank());
		}

		void CylinderFluidTank::attachToStage() {
			mStageLink = std::make_shared<chrono::ChLinkLockLock>();
			mSystemHandle.AddLink(mStageLink);
			
			// test that this is working as expected
			//mStageLink->Initialize(mBody, mStageBodyHandle, mComp_to_stage.GetCoord());
			mStageLink->Initialize(mBody, mStageBodyHandle, chrono::ChCoordsys(chrono::Vector(0, 100, 0)));

			// Why is this a) causing the simulation to take so long and b) the stage to disappear because of a nan position?
			// --------------------------------------------------------------------------------------------------------------
		}

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
			ChMatrix33<> tankInertiaVoid_tankCoM = utils::CalcCylinderGyration(mRadius - mWallThickness, mHeight * 0.5 - mWallThickness * 2);
			
			// Place this inertia at the tank's CoM (relative to the tank's origin) and make it a void material
			tankInertia_tank.AddComponent(mTankCoM, mTankMass, tankInertiaSolid_tankCoM, true);

			// The final result should contain the inertia of a thick-walled but hollow cylinder, about the origin of the tank (ie. base of cylinder)
			// For a stage component, this is the correct form and space for the inertia to be in
			return tankInertia_tank.GetInertiaReference();
		}

		void CylinderFluidTank::onFluidMassChange() 
			// Responsible for updating various tank properties after the fluid mass has changed
		{
			mFluidVolume = mFluidMass / mFluid.mDensity;
			mFluidLevel = (mFluidVolume / mVolume_internal) * mInternalHeight;
			mPercentFull = mFluidMass / mMaxFluidMass;

			// addFluid() and removeFluid() handle the change of value of the fluid mass,
			// but it is up to this function to then update the centre of mass after this change
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