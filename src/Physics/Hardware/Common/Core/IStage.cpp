#include "IStage.h"

namespace Physics {
	namespace Hardware {

		void IStage::update(double t, double dt) {
			assert(mInitialised && "Stage must be initialised before it is updated.");

			mEngines.update(dt);
			mThrusters.update(dt);
			mPropellantSupplies.update();
			otherUpdates(t, dt); //Updates any other custom features of a stage (e.g. Grid_fins, landing legs etc)

			mState.setMass_local(recalcTotalMass_stage());
			mState.setInertiaTensor_local(recalcCmInertia_stage());

			integrate(t, dt);

			basicCollision();

			mFlowVelocity_world = External::Environment::windVelocity_m_per_s - mState.getVelocity_world();
			mFlowVelocity_local = glm::inverse(mState.getOrientation_world()) * mFlowVelocity_world;
			//mDynamicPressure = 0.5 * External::Environment::getAirDensity(floor(mState.getPosition_world().y)) * pow(glm::length(mFlowVelocity_local), 2.0);
			//updateAngleAttack();
		}

		void IStage::initState()
			//This function must be called by sub classes to set the initial state of the stage before any updates take place. 
		{
			mInitialised = true;
			update(0.0, 0.0);
		}

		void IStage::addForces(const State &state, double t) {
#if !STAGE_FORCE_ACTIVE
			return;
#endif
			//Weight
#if STAGE_WEIGHT_ACTIVE
			addWorldForceThroughCM(glm::dvec3(0.0, mState.getMass_local().getValue() * -External::Environment::getGravity(static_cast<int>(floor(mState.getCMPosition_world().y))), 0.0));
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
			for (const Force_world& force : otherForces_world())
				addForce(force);
		}

		void IStage::addTorques(const State &state, double t) { }

		Mass IStage::recalcTotalMass_stage() const {
			return
				mPropellantSupplies.getTotalMass_stage() +
				mEngines.getTotalMass_stage() +
				mThrusters.getTotalMass_stage() +
				mMiscInertMass +
				otherMass_stage();
		}

		InertiaTensor IStage::recalcCmInertia_stage() const {
			

			glm::dvec3 stageCentreMass_stage = mState.getMass_local().getCentre();

			//Adding the moment of inertia of the all tanks
			glm::dvec3 displacement = mPropellantSupplies.getTotalMass_stage().getCentre() - stageCentreMass_stage;
			double mass = mPropellantSupplies.getTotalMass_stage().getValue();
			InertiaTensor output = InertiaTensor::parallelAxis(mPropellantSupplies.getTotalCmInertia_stage(), mass, displacement);

			//Adding the moment of inertia of the engines
			displacement = mEngines.getTotalMass_stage().getCentre() - stageCentreMass_stage;
			mass = mEngines.getTotalMass_stage().getValue();
			output += InertiaTensor::parallelAxis(mEngines.getTotalCmInertia_stage(), mass, displacement);

			//Adding the moment of inertia of the thrusters
			displacement = mThrusters.getTotalMass_stage().getCentre() - stageCentreMass_stage;
			mass = mThrusters.getTotalMass_stage().getValue();
			output += InertiaTensor::parallelAxis(mThrusters.getTotalCmInertia_stage(), mass, displacement);

			//Adding the moment of any custom objects (e.g. landing legs, grid fins) that come from specific stages
			displacement = otherMass_stage().getCentre() - stageCentreMass_stage;
			mass = otherMass_stage().getValue();
			output += InertiaTensor::parallelAxis(otherCmInertia_stage(), mass, displacement);

			return output;
		}

		//void IStage::updateAngleAttack() {
		//	using namespace glm;
		//
		//	mAoADegs_pitch = degrees(angle(normalize(mState.getOrientation_world() * mStartDirection_world), normalize(mState.getVelocity_world())));
		//	if (mAoADegs_pitch > 90.0)
		//		mAoADegs_pitch = 90.0 - (mAoADegs_pitch - 90.0);
		//
		//	mAoADegs_flow = degrees(angle(mStartDirection_world, normalize(mFlowVelocity_stage)));
		//	if (mAoADegs_flow > 90.0)
		//		mAoADegs_flow = 90.0 - (mAoADegs_flow - 90.0);
		//}

		void IStage::basicCollision() {
			glm::dvec3 currentPosition = mState.getObjectSpace().toParentSpace();

			double groundHeight = 0.0; //3.0

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