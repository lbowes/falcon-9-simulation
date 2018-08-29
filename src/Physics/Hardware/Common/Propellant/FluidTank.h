#ifndef PHYSICS_HARDWARE_FLUIDTANK_H
#define PHYSICS_HARDWARE_FLUIDTANK_H

#include "Physics/Hardware/Common/Core/IStageComponent.hpp"
#include "Fluid.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include <algorithm>
#include <glm/gtx/transform.hpp>

namespace Physics {
	namespace Hardware {
		
		class FluidTank : public IStageComponent {
			friend class FluidTankGroup;
		private:
			const Fluid mFluid;

			const double
				mHeight = 0.0,            //m
				mDiameter = 0.0,          //m
				mWallThickness = 0.0,     //m
				mMaterialDensity = 0.0;   //kg/m^3

			double
				mPropellantVolume = 0.0,  //m^3
				mPropellantLevel = 0.0,   //m, measured from the top of the lower wall of the tank (= 0.0) 
				mMaxPropellantMass = 0.0, //kgs
				mInternalVolume = 0.0,    //m^3
				mInternalHeight = 0.0,    //m
				mPercentFull = 0.0;       

			Mass
				mTankMass_tank,
				mPropellantMass_tank;

			InertiaTensor
				mTankInertia_tankCoM,
				mPropellantInertia_propCoM;

		public:
			FluidTank(GF::CoordTransform3D tankToStage, Fluid fluid, double height, double diameter, double wallThickness, double materialDensity);
			~FluidTank() = default;

			void addPropellant(double mass_kgs);
			void removePropellant(double mass_kgs);
			void removeAllPropellant();
			
			bool isEmpty() const { return mPropellantMass_tank.getValue() <= 0.0; }
			double getPropMassValue_tank() const { return mPropellantMass_tank.getValue(); }
			double getPercentFull() const { return mPercentFull; }

		private:
			void init();
			void onPropMassValueChange();
			void updateComponentState();

		};

	}
}

#endif
