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

		void Engine::addTVCActuator(glm::dvec2 fixedPoint_engine, glm::dvec2 engineConnectPoint_engine, double clockingDegree_degs) {
			mTVCActuators.push_back(TVCActuator(mMaxGimbalAngle, fixedPoint_engine, engineConnectPoint_engine, clockingDegree_degs));
		}

		void Engine::updateDeviceSpecific(double dt) {
			if(!mTVCActuators.empty())
				updateGimbalRotation();

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

		void Engine::updateGimbalRotation() 
			//Responsible for updating the mCompToStage for gimballing rotation, given the current state of all the mTVCActuators
		{
			using namespace glm;

			//This transform is in 'neutral engine' space, ie engine space without any gimballing
			dmat4 gimbalTransform_engine;
			for (const auto& t : mTVCActuators) {
				gimbalTransform_engine = 
				rotate(
					gimbalTransform_engine, 
					radians(t.mResultantAngle), 
					cross(
						{0.0, 1.0, 0.0}, 
						rotate(
							{0.0, 0.0, -1.0}, 
							radians(t.mClockingDegree), 
							dvec3(0.0, 1.0, 0.0)
						)
					)
				);
			}

			//Update the component to stage transform
			mCompToStage.setLocalToParent_rotation(gimbalTransform_engine * mEngineToStageNoGimbal.getLocalToParent_rotation());
		}

	}
}