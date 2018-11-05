#include "Falcon9.h"

namespace Physics {
	namespace Hardware {

		Falcon9::Falcon9() :
			ILaunchVehicle({ "LC-39A", { 0.0, 0.0, 0.0 } })
		{
			assemble();
		}

		void Falcon9::update(double t, double dt) {
			//temp - added to add some pseudo control code to the simulation to make it interesting for debugging
			
			if(t > 10.0) {
				for(unsigned char i = 0; i < mStage1.getEngines().getCount(); i++) {
					static_cast<Physics::Hardware::Engine*>(mStage1.getEngines()[i])->setActive(true);
					static_cast<Physics::Hardware::Engine*>(mStage1.getEngines()[i])->setThrottle(1.0);
				}
			}

			static_cast<GasThruster*>(mStage1.getThrusters()[0])->setActive(false);
			const double kickDuration = 0.3;
			if(mState.getCoMPosition_world().y > 50.0) {
				static_cast<GasThruster*>(mStage1.getThrusters()[0])->setActive(true);
				static double pitchKickTime = t;

				if(t - pitchKickTime > kickDuration)
					static_cast<GasThruster*>(mStage1.getThrusters()[0])->setActive(false);
			}

			static_cast<GasThruster*>(mStage1.getThrusters()[4])->setActive(false);
			if(t - 15.0 > 2.0) {
				static_cast<GasThruster*>(mStage1.getThrusters()[4])->setActive(true);
				static double pitchCorrectionTime = t;

				if(t - pitchCorrectionTime > kickDuration - 0.2)
					static_cast<GasThruster*>(mStage1.getThrusters()[4])->setActive(false);
			}
			//

			mStage1.update(t, dt);
			mStage2.update(t, dt);

			ILaunchVehicle::update(t, dt);
		}

		void Falcon9::loadDynamicState(const DSS::Falcon9& state) {
			DSS::loadRigidBodyState(state.RB, mState);
			
			mInertialPosition = state.inertialPosition;

			mStage1.loadDynamicState(state.S1);
			mStage2.loadDynamicState(state.S2);
		}

		void Falcon9::saveDynamicState(DSS::Falcon9& toSaveTo) const {
			DSS::saveRigidBodyState(*this, toSaveTo.RB);
			
			toSaveTo.inertialPosition = mInertialPosition;

			mStage1.saveDynamicState(toSaveTo.S1);
			mStage2.saveDynamicState(toSaveTo.S2);
		}

		void Falcon9::assemble() {
			addBody(mStage1);
			addBody(mStage2, glm::dvec3(0.0, 45.76182, 0.0));
		}

	}
}