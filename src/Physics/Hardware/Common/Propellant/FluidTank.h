#ifndef PHYSICS_HARDWARE_FLUIDTANK_H
#define PHYSICS_HARDWARE_FLUIDTANK_H

#include "Physics/Hardware/Common/Core/StageComponent.hpp"
#include "Fluid.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include <algorithm>
#include <glm/gtx/transform.hpp>

namespace Physics {
	class DSS;
}

namespace Physics {
	namespace Hardware {
		
		class FluidTank : public StageComponent {
			friend class FluidTankGroup;
			friend class Physics::DSS;
		private:
			const Fluid mFluid;

			const double
				mHeight = 0.0,          //m
				mDiameter = 0.0,        //m
				mWallThickness = 0.0,   //m
				mMaterialDensity = 0.0, //kg/m^3
				mInternalHeight = 0.0,  //m
				mInternalVolume = 0.0,  //m^3
				mMaxFluidMass = 0.0;    //kgs

			const Mass mTankMass_tank;
			
			const InertiaTensor mTankInertia_tankCoM;
			
			double
				mFluidVolume = 0.0,     //m^3
				mFluidLevel = 0.0,      //m, measured from the top of the lower wall of the tank (= 0.0) 
				mPercentFull = 0.0;

			Mass mFluidMass_tank;

			InertiaTensor mFluidInertia_fluidCoM;

		public:
			FluidTank(CoordTransform3D tankToStage, Fluid fluid, double height, double diameter, double wallThickness, double materialDensity);
			~FluidTank() = default;

			void addFluid(double mass);
			void removeFluid(double mass);
			void removeAllFluid();

			bool isEmpty() const { return mFluidMass_tank.getValue() <= 0.0; }
			double getFluidMassValue_tank() const { return mFluidMass_tank.getValue(); }
			double getPercentFull() const { return mPercentFull; }

		private:
			void onFluidMassChange();
			void updateComponentState();

		};

	}
}

#endif
