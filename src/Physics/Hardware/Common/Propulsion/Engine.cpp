#include "Engine.h"

namespace Physics {
	namespace Hardware {

		Engine::Engine(CoordTransform3D engineToStage) :
			IThrustGenerator(engineToStage),
			mEngineToStageNoGimbal(engineToStage)
		{ }

		void Engine::setThrottle(double newThrottle) {
			if (newThrottle <= mThrottleMax && newThrottle >= mThrottleMin)
				mThrottle = newThrottle;
		}

		void Engine::updateProperties() 
			//Updates 'secondary' values from primary ones. E.g. recalculating nozzle exit area after the nozzle diameter is set
		{
			mNozzleExitArea = glm::pi<double>() * pow(mNozzleExitDiameter / 2.0, 2);
			mNozzleThroatArea = mNozzleExitArea / mExpansionRatio;
		}

		void Engine::updateDeviceSpecific(double dt) {
			updateGimbalAngle();

			bool propDemandMet = false;

			if (mSupplyLine && mActive) {
				double 
					requiredOxidiserMass = mOFMixtureRatio / (mOFMixtureRatio + 1.0) * mCurrentMassFlowRate * dt,
					requiredFuelMass = 1.0 / (mOFMixtureRatio + 1.0) * mCurrentMassFlowRate * dt,
					currentOxidiserMass = mSupplyLine->transferOxidiser(requiredOxidiserMass), //Attempt to pull the required propellant from the supply...
					currentFuelMass = mSupplyLine->transferFuel(requiredFuelMass);

				//Check whether enough propellant was transferred...
				propDemandMet =
					(currentOxidiserMass >= requiredOxidiserMass) &&
					(currentOxidiserMass > 0.0) &&
					(currentFuelMass >= requiredFuelMass) &&
					(currentFuelMass > 0.0);
			}

			//If there isnt't enough propellant, deactivate the engine
			if (!propDemandMet)
				mActive = false;

			//Calculate the thrust properties of the engine
			mThrustMagnitude = mPeakThrustMagnitude * mThrottle;
			mCurrentMassFlowRate = mPeakMassFlowRate * mThrottle;
		}

		void Engine::updateGimbalAngle() {
			//TODO:
			//mCompToStage.setLocalToParent_rotation();






		}

	}
}