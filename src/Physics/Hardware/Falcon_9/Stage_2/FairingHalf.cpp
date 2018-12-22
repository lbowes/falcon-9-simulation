#include "FairingHalf.h"

#include <iostream>

namespace Physics {
	namespace Hardware { 

		FairingHalf::FairingHalf() { 
			mState.setMass_local(recalcMass_local());
			mState.setInertiaTensor_local(recalcInertia_local());
		}

		void FairingHalf::update(double t, double dt) {
			integrate(t, dt);
			basicCollision_temp();
		}
		
		void FairingHalf::addForces(const State& state, double t) { 
			//TODO: Gravity
			addWorldForceThroughCM(glm::dvec3(0.0, mState.getMass_local().getValue() * -External::Environment::getGravity(static_cast<int>(floor(mState.getCoMPosition_world().y))), 0.0));
		}

		void FairingHalf::addTorques(const State& state, double t) { }

		void FairingHalf::basicCollision_temp() {
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

		Mass FairingHalf::recalcMass_local() const {
			//TODO: Just an estimate
			return Mass(1000.0, {0.0, mCentreMassPos_fairing2D.y, -mCentreMassPos_fairing2D.x});
		}
		
		InertiaTensor FairingHalf::recalcInertia_local() const {
			//TODO: Currently unknown
			return InertiaTensor();
		}

	}
}