#include "CylinderFluidTank.h"

#include <chrono/utils/ChCompositeInertia.h>
#include <chrono/utils/ChUtilsGeometry.h>

namespace Physics {
	namespace Hardware {

		CylinderFluidTank::CylinderFluidTank(const chrono::ChFrame<>& tank_to_Stage, Fluid fluid, double height, double radius, double wallThickness, double materialDensity) :
			mTank_to_Stage(tank_to_Stage),
			mFluid(fluid),
			mHeight(height),
			mRadius(radius),
			mWallThickness(wallThickness),
			mMaterialDensity(materialDensity),
			mInternalHeight(mHeight - 2.0 * mWallThickness),
			mVolume_internal(chrono::utils::CalcCylinderVolume(radius - wallThickness, mInternalHeight * 0.5)),
			mVolume_external(chrono::utils::CalcCylinderVolume(radius, mHeight * 0.5)),
			mMaxFluidMass(mFluid.mDensity * mVolume_internal),
			mTankMass(mMaterialDensity * (mVolume_external - mVolume_internal)),
			mTankCoM_tank({0, height * 0.5, 0}),
			mTankInertia_tank(tankInertia_tank())
		{ }

		// Could these two functions be combined into one 'changeFluidMass()' function with positive or negative arguments and std::clamp??
		void CylinderFluidTank::addFluid(double mass) {
			mFluidMass = std::min(mFluidMass + mass, mMaxFluidMass);
			onFluidMassChange();
		}

		void CylinderFluidTank::removeFluid(double mass) {
			mFluidMass = std::max(mFluidMass - mass, 0.0);
			onFluidMassChange();
		}
		//

		void CylinderFluidTank::removeAllFluid() {
			mFluidMass = 0.0;
			onFluidMassChange();
		}

		chrono::ChFrame<> CylinderFluidTank::calcTransform_toStage() const {
			return mTank_to_Stage;
		}

		chrono::ChMatrix33<> CylinderFluidTank::calcInertia_comp() const {
			// Fluid inertia in tank space
			return mTankInertia_tank + mFluidInertia_tank;
		}

		double CylinderFluidTank::calcMass() const {
			// Total mass of tank structure and fluid
			return mTankMass + mFluidMass;
		}

		chrono::ChVector<> CylinderFluidTank::calcCoM_comp() const {
			// Combined CoM of the tank structure and (very simply modelled) fluid within
			return (mTankMass * mTankCoM_tank + mFluidMass * mFluidCoM_tank) / (mTankMass + mFluidMass);
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
		}

	}
}