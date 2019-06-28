#ifndef PHYSICS_HARDWARE_CYLINDERFLUIDTANK_H
#define PHYSICS_HARDWARE_CYLINDERFLUIDTANK_H
#pragma once

#include "Fluid.h"
#include "../../../../IDataSource.hpp"

#include <chrono/physics/ChBodyAuxRef.h>

namespace chrono {
    class ChSystemNSC;
}

namespace Physics {
	namespace Hardware {

		class CylinderFluidTank : public IDataSource {
		private:
            std::shared_ptr<chrono::ChBodyAuxRef> mBody;
            
            chrono::ChFrame<> mTransform_stage;
			
			const Fluid mFluid;

			const double
				mHeight,          // m
				mRadius,          // m
				mWallThickness,   // m
				mMaterialDensity, // kg/m^3
				mInternalHeight,  // m
				mVolume_internal, // m^3 volume inside tank excluding tank walls
				mMaxFluidMass,    // kg
				mTankMass;        // kg

			const chrono::Vector mTankCoM_tank;

			const chrono::ChMatrix33<> mTankInertia_tank;

			double
				mFluidVolume,     // m^3
				mFluidLevel,      // m, measured from the top of the lower wall of the tank (= 0.0) 
				mPercentFull,     // 0 -> 1
				mFluidMass;       // kg

			chrono::Vector mFluidCoM_tank;

			chrono::ChMatrix33<> mFluidInertia_tank;

		public:
			CylinderFluidTank(chrono::ChSystemNSC& system, Fluid f, double height, double radius, double thickness, double density);
			~CylinderFluidTank() = default;

            //void attachToStage();
			void addFluid(double mass);
			void removeFluid(double mass);
			void removeAllFluid();
            void outputToCSV(std::string& destRowCSV) const override;

			bool isEmpty() const { return mFluidMass <= 0.0; }
			double getFluidMassValue_tank() const { return mFluidMass; }
			double getPercentFull() const { return mPercentFull; }

		private:
			void assemble(chrono::ChSystemNSC& system);
            double combinedMass() const;
			chrono::ChFrame<> combinedCoM_tank() const;
			chrono::ChMatrix33<> combinedInertia_tank() const;
			chrono::ChMatrix33<> tankInertia_tank() const;
			void onFluidMassChange();

		};

	}
}

#endif // PHYSICS_HARDWARE_CYLINDERFLUIDTANK_H
