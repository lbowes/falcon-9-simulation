#include "GasThruster.h"

namespace Physics {
	namespace Hardware {

		GasThruster::GasThruster(GF::CoordTransform3D engineToStage) :
			IThrustGenerator(engineToStage)
		{
			setTypeSpecificParams();
		}

		void GasThruster::setTypeSpecificParams() {
			//TODO - The thrust of these gas thrusters is not this value. This is a placeholder value for testing.
			mThrustMagnitude = /*7'000.0*//*2'000'000.0*/500'000.0;  //7000.0 might be close to actual
			mCurrentMassFlowRate = 0.018;
			mMass_comp = 0.174;
		}

		void GasThruster::updateDeviceSpecific(double dt) { 
			bool propDemandMet = false;

			if (mSupplyLine && mActive) {
				double 
					requiredGasMass = mCurrentMassFlowRate * dt,
					currentGasMass = mSupplyLine->transferGas(requiredGasMass);

				propDemandMet = (currentGasMass >= requiredGasMass) && (currentGasMass > 0.0);
			}

			//If there isnt't enough propellant, deactivate the thruster
			if (!propDemandMet)
				mActive = false;
		}

	}
}