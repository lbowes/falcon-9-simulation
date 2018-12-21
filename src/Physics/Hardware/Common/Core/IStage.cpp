#include "IStage.h"

namespace Physics {
	namespace Hardware {

		void IStage::update(double t, double dt) {
			mEngines.update(dt);
			mThrusters.update(dt);
			mPropellantSupplies.update();
			stageSpecificUpdates(t, dt);   //Updates any other custom features of a stage (e.g. Grid fins, landing legs etc)

			mergeTotalMass_stage();
			mergeTotalInertia_stage();

			integrate(t, dt);

			basicCollision_temp();
		}

		void IStage::addForces(const State &state, double t) {
#if !STAGE_FORCE_ACTIVE
			return;
#endif
			//Weight
#if STAGE_WEIGHT_ACTIVE
			addWorldForceThroughCM(glm::dvec3(0.0, mState.getMass_local().getValue() * -External::Environment::getGravity(static_cast<int>(floor(mState.getCoMPosition_world().y))), 0.0));
#endif
			//Engines
			IThrustGenerator* thrustGenerator = nullptr;
			for (const auto& e : mEngines.getAllComponents()) {
				thrustGenerator = static_cast<IThrustGenerator*>(e.get());
				addLocalForceAtLocalPoint(thrustGenerator->getThrust_stage(), thrustGenerator->getCompToStageTransform().toParentSpace());
			}
			
			//Thrusters
			for (const auto& t : mThrusters.getAllComponents()) {
				thrustGenerator = static_cast<IThrustGenerator*>(t.get());
				addLocalForceAtLocalPoint(thrustGenerator->getThrust_stage(), thrustGenerator->getCompToStageTransform().toParentSpace());
			}

			//Extra forces
			for (const Force_world& force : stageSpecificForces_world())
				addForce(force);
		}

		void IStage::addTorques(const State &state, double t) { }

		void IStage::mergeTotalMass_stage() {
			Mass total = 
				mPropellantSupplies.getTotalMass_stage() +
				mEngines.getTotalMass_stage() +
				mThrusters.getTotalMass_stage() +
				mMiscInertMass +
				stageSpecificMass_stage();

			mState.setMass_local(total);
		}

		void IStage::mergeTotalInertia_stage() {
			glm::dvec3 stageCentreMass_stage = mState.getMass_local().getCentre();

			//Inertia of the all tanks
			glm::dvec3 displacement = mPropellantSupplies.getTotalMass_stage().getCentre() - stageCentreMass_stage;
			double mass = mPropellantSupplies.getTotalMass_stage().getValue();
			InertiaTensor total = InertiaTensor::parallelAxis(mPropellantSupplies.getTotalCoMInertia_stage(), mass, displacement);

			//Inertia of the engines
			displacement = mEngines.getTotalMass_stage().getCentre() - stageCentreMass_stage;
			mass = mEngines.getTotalMass_stage().getValue();
			total += InertiaTensor::parallelAxis(mEngines.getTotalCoMInertia_stage(), mass, displacement);

			//Inertia of the thrusters
			displacement = mThrusters.getTotalMass_stage().getCentre() - stageCentreMass_stage;
			mass = mThrusters.getTotalMass_stage().getValue();
			total += InertiaTensor::parallelAxis(mThrusters.getTotalCoMInertia_stage(), mass, displacement);

			//Inertia of any custom objects (e.g. landing legs, grid fins) that come with specific stages
			displacement = stageSpecificMass_stage().getCentre() - stageCentreMass_stage;
			mass = stageSpecificMass_stage().getValue();
			total += InertiaTensor::parallelAxis(stageSpecificCoMInertia_stage(), mass, displacement);

			mState.setInertiaTensor_local(total);
		}

		void IStage::basicCollision_temp() {
			glm::dvec3 currentPosition = mState.getObjectSpace().toParentSpace();

			double groundHeight = 0.0;

			if (currentPosition.y < groundHeight) {
				currentPosition.y += groundHeight - currentPosition.y;

				mState.setPosition_world(currentPosition);

				glm::dvec3 temp = mState.getVelocity_world();
				temp.y = 0.0;
				mState.setVelocity_world(temp);

				temp = mState.getMomentum_world();
				temp.y = 0.0;
				mState.setMomentum_world(temp);
			}
		}

	}
}