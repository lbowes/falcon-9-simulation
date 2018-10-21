#include "FluidTank.h"

#include <iostream>

namespace Physics {
	namespace Hardware {
		
		FluidTank::FluidTank(CoordTransform3D tankToStage, Fluid fluid, double height, double diameter, double wallThickness, double materialDensity) :
			IStageComponent(tankToStage),
			mFluid(fluid),
			mHeight(height),
			mDiameter(diameter),
			mWallThickness(wallThickness),
			mMaterialDensity(materialDensity),
			mMaxFluidMass(mFluid.mDensity * mInternalVolume),
			mInternalHeight(mHeight - 2.0 * mWallThickness),
			mInternalVolume(M_PI * pow(mDiameter * 0.5 - mWallThickness, 2.0) * (mHeight - 2.0 * mWallThickness)),
			mTankMass_tank(Mass(mMaterialDensity * ((M_PI * pow(mDiameter * 0.5, 2.0) * mHeight) - mInternalVolume), { 0.0, 0.5 * mHeight, 0.0 })),
			mTankInertia_tankCoM(InertiaTensor::thickWalledCylinder(mTankMass_tank.getValue(), mDiameter, mWallThickness, mHeight))
		{
			updateComponentState();
		}
		
		void FluidTank::addFluid(double mass) {
			double newMass = mFluidMass_tank.getValue() + mass;

			if (newMass <= mMaxFluidMass)
				mFluidMass_tank += mass;
			else
				mFluidMass_tank = mMaxFluidMass;

			onFluidMassChange();
		}

		void FluidTank::removeFluid(double mass) {
			mFluidMass_tank = std::max((mFluidMass_tank - mass).getValue(), 0.0);
			onFluidMassChange();
		}

		void FluidTank::removeAllFluid() {
			mFluidMass_tank = 0.0;
			onFluidMassChange();
		}

		void FluidTank::loadDynamicState(const DSS::FluidTankState& state) {
			mFluidVolume = state.fluidVolume;
			mFluidLevel = state.fluidLevel;
			mPercentFull = state.percentFull;
			mMass_comp = state.mass_comp;
			mFluidMass_tank = state.fluidMass_tank;
			mCoMInertia_comp = state.CoMInertia_comp;
			mFluidInertia_fluidCoM = state.fluidInertia_fluidCoM;
		}

		void FluidTank::saveDynamicState(DSS::FluidTankState& toSaveTo) const {
			toSaveTo.fluidVolume = mFluidVolume;
			toSaveTo.fluidLevel = mFluidLevel;
			toSaveTo.percentFull = mPercentFull;
			toSaveTo.mass_comp = mMass_comp;
			toSaveTo.fluidMass_tank = mFluidMass_tank;
			toSaveTo.CoMInertia_comp = mCoMInertia_comp;
			toSaveTo.fluidInertia_fluidCoM = mFluidInertia_fluidCoM;
		}

		void FluidTank::onFluidMassChange() 
			//Responsible for updating various tank properties after the fluid mass has changed.
		{
			mFluidVolume = mFluidMass_tank.getValue() / mFluid.mDensity;
			mFluidLevel = (mFluidVolume / mInternalVolume) * mInternalHeight;
			mPercentFull = mFluidMass_tank.getValue() / mMaxFluidMass;

			//addFluid() and removeFluid() handle the change of value of the fluid mass,
			//but it is up to this function to then refresh the centre of mass after this change
			if (mFluid.mState == FluidState::liquid)
				mFluidMass_tank.setCentre(glm::dvec3(0.0, mWallThickness + mFluidLevel * 0.5, 0.0));
			else if(mFluid.mState == FluidState::gas)
				mFluidMass_tank.setCentre(glm::dvec3(0.0, mWallThickness + mInternalHeight * 0.5, 0.0));

			//After the fluid level has changed, the inertia about its centre of mass is updated. 
			mFluidInertia_fluidCoM = InertiaTensor::solidCylinder(mFluidMass_tank.getValue(), mDiameter - 2 * mWallThickness, mFluidLevel);

			updateComponentState();
		}
		
		void FluidTank::updateComponentState() {
			//This updates the total centre of mass of the system in tank space.
			mMass_comp = mTankMass_tank + mFluidMass_tank;

			//The centre of mass of the system then needs to be used in order to calculate the total inertia about this centre of mass.
			//Since neither the tank or the fluid will lie directly on the centre of mass, the parallel axis theorem needs to be
			//used for both.

			//Tank
			mCoMInertia_comp = InertiaTensor::parallelAxis(mTankInertia_tankCoM, mTankMass_tank.getValue(), mTankMass_tank.getCentre() - mMass_comp.getCentre());

			//Fluid
			mCoMInertia_comp += InertiaTensor::parallelAxis(mFluidInertia_fluidCoM, mFluidMass_tank.getValue(), mFluidMass_tank.getCentre() - mMass_comp.getCentre());
		}

	}
}