#ifndef PHYSICS_HARDWARE_CYLINDERFLUIDTANK_H
#define PHYSICS_HARDWARE_CYLINDERFLUIDTANK_H
#pragma once

#include "../Structural/IStageComponent.hpp"
#include "Fluid.h"

namespace Physics {
	namespace Hardware {

		class CylinderFluidTank : public IStageComponent {
		private:
			chrono::ChFrame<> mTank_to_Stage;
			
			const Fluid mFluid;
			
			const double
				mHeight,          //m
				mRadius,          //m
				mWallThickness,   //m
				mMaterialDensity, //kg/m^3
				mInternalHeight,  //m
				mVolume_internal, //m^3 volume inside tank excluding tank walls
				mVolume_external, //m^3 volume including tank walls
				mMaxFluidMass,    //kgs
				mTankMass;        //kgs

			const chrono::ChVector<> mTankCoM_tank;

			const chrono::ChMatrix33<> mTankInertia_tank;

			double
				mFluidVolume,     //m^3
				mFluidLevel,      //m, measured from the top of the lower wall of the tank (= 0.0) 
				mPercentFull,     //0->1
				mFluidMass;       //kgs

			chrono::ChVector<> mFluidCoM_tank;

			chrono::ChMatrix33<> mFluidInertia_tank;

		public:
			CylinderFluidTank(const chrono::ChFrame<>& tank_to_Stage, Fluid fluid, double height, double radius, double wallThickness, double materialDensity);
			~CylinderFluidTank() = default;

			void addFluid(double mass);
			void removeFluid(double mass);
			void removeAllFluid();

			bool isEmpty() const { return mFluidMass <= 0.0; }
			double getFluidMassValue_tank() const { return mFluidMass; }
			double getPercentFull() const { return mPercentFull; }

		private:
			chrono::ChFrame<> calcTransform_toStage() const override;
			chrono::ChMatrix33<> calcInertia_comp() const override;
			double calcMass() const override;
			chrono::ChVector<> calcCoM_comp() const override;
			chrono::ChMatrix33<> tankInertia_tank() const;
			void onFluidMassChange();

		};

	}
}

#endif
