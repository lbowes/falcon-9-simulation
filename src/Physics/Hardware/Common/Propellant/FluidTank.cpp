#include "FluidTank.h"

namespace Physics {
	namespace Hardware {
		
		FluidTank::FluidTank(GF::CoordTransform3D tankToStage, Fluid fluid, double height, double diameter, double wallThickness, double materialDensity) :
			IStageComponent(tankToStage),
			mFluid(fluid),
			mHeight(height),
			mDiameter(diameter),
			mWallThickness(wallThickness),
			mMaterialDensity(materialDensity)
		{
			init();
		}
		
		void FluidTank::addPropellant(double mass_kgs) {
			double targetMass = mPropellantMass_tank.getValue() + mass_kgs;

			if (targetMass <= mMaxPropellantMass)
				mPropellantMass_tank += mass_kgs;
			else
				mPropellantMass_tank = mMaxPropellantMass;

			onPropMassValueChange();
		}

		void FluidTank::removePropellant(double mass_kgs) {
			mPropellantMass_tank = std::max((mPropellantMass_tank - mass_kgs).getValue(), 0.0);
			onPropMassValueChange();
		}

		void FluidTank::removeAllPropellant() {
			mPropellantMass_tank = 0.0;
			onPropMassValueChange();
		}

		void FluidTank::init() {
			
			using namespace glm;

			mInternalVolume = (M_PI * pow(mDiameter * 0.5 - mWallThickness, 2.0) * (mHeight - 2.0 * mWallThickness));
			mMaxPropellantMass = mFluid.mDensity * mInternalVolume;
			mInternalHeight = (mHeight - 2.0 * mWallThickness);

			mTankMass_tank = Mass(mMaterialDensity * ((M_PI * pow(mDiameter * 0.5, 2.0) * mHeight) - mInternalVolume), glm::dvec3(0.0, 0.5 * mHeight, 0.0));
			mTankInertia_tankCoM = InertiaTensor::thickWalledCylinder(mTankMass_tank.getValue(), mDiameter, mWallThickness, mHeight);
			
			updateComponentState();
		}

		void FluidTank::onPropMassValueChange() 
			//Responsible for updating various propellant properties after the mass has changed.
		{
			mPropellantVolume = mPropellantMass_tank.getValue() / mFluid.mDensity;
			mPropellantLevel = (mPropellantVolume / mInternalVolume) * mInternalHeight;
			mPercentFull = mPropellantMass_tank.getValue() / mMaxPropellantMass;

			//addPropellant() and removePropellant() handle the change of value of the propellant mass,
			//but it is up to this function to then refresh the centre of mass after this change
			if (mFluid.mState == FluidState::liquid)
				mPropellantMass_tank.setCentre(glm::dvec3(0.0, mWallThickness + mPropellantLevel * 0.5, 0.0));
			else if(mFluid.mState == FluidState::gas)
				mPropellantMass_tank.setCentre(glm::dvec3(0.0, mWallThickness + mInternalHeight * 0.5, 0.0));

			//After the propellant level has changed, the inertia about its centre of mass is updated. 
			mPropellantInertia_propCoM = InertiaTensor::solidCylinder(mPropellantMass_tank.getValue(), mDiameter - 2 * mWallThickness, mPropellantLevel);

			updateComponentState();
		}
		
		void FluidTank::updateComponentState() {
			

			//This updates the total centre of mass of the system in tank space.
			mMass_comp = mTankMass_tank + mPropellantMass_tank;

			//The centre of mass of the system then needs to be used in order to calculate the total inertia about this centre of mass.
			//Since neither the tank or the propellant will lie directly on the centre of mass, the parallel axis theorem needs to be
			//used for both.

			//Tank
			mCMInertia_comp = InertiaTensor::parallelAxis(mTankInertia_tankCoM, mTankMass_tank.getValue(), mTankMass_tank.getCentre() - mMass_comp.getCentre());

			//Propellant
			mCMInertia_comp += InertiaTensor::parallelAxis(mPropellantInertia_propCoM, mPropellantMass_tank.getValue(), mPropellantMass_tank.getCentre() - mMass_comp.getCentre());
		}

	}
}