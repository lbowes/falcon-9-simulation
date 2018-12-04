#include "Falcon9.h"

namespace Physics {
	namespace Hardware {

		Falcon9::Falcon9() :
			ILaunchVehicle({ "LC-39A", { 0.0, 0.0, 0.0 } })
		{
			assemble();
		}

		void Falcon9::update(double t, double dt) {
			//temp
			debugControlCode(t, dt);
			//

			mStage1.update(t, dt);
			mStage2.update(t, dt);

			ILaunchVehicle::update(t, dt);
		}

		void Falcon9::separateStages() {
			mMembers[0].disconnect();
			mMembers[1].disconnect();
		}

		void Falcon9::debugControlCode(double t, double dt) {
			//temp - added to add some pseudo control code to the simulation to make it interesting for debugging

#define LANDING 0
#define LAUNCH  1

#if LANDING
			mStage1.getEngines().getComponent<Engine>(0)->setActive(true);

			for(unsigned char i = 0; i < mStage1.getGridFins().getCount(); i++)
				mStage1.getGridFins().getComponent<GridFin>(i)->deploy();

			if(mStage1.getState().getCoMPosition_world().y < 250.0 && t > 10.0) {
				for(unsigned char i = 0; i < mStage1.getLandingLegs().getCount(); i++)
					mStage1.getLandingLegs().getComponent<LandingLeg>(i)->deploy();
			}

			const double 
				activeTime = 17.6,
				duration = 0.01;

			static_cast<GasThruster*>(mStage1.getThrusters()[0])->setActive(t > activeTime && t < activeTime + duration);
			static_cast<GasThruster*>(mStage1.getThrusters()[4])->setActive(t > activeTime + 2.4 && t < activeTime + 2.4 + duration);

			if(abs(mStage1.getState().getVelocity_world().y) <= 0.1) {
				mStage1.getState().setVelocity_world({});
				mStage1.getState().setOrientation_world({});
				mStage1.getEngines().getComponent<Engine>(0)->setActive(false);
			}
#elif LAUNCH
			if(t > 5.0) {
				for(unsigned char i = 0; i < 9; i++)
					mStage1.getEngines().getComponent<Engine>(i)->setActive(true);
			}
#endif
		}

		void Falcon9::assemble() {
			addBody(mStage1);
			addBody(mStage2, glm::dvec3(0.0, 45.76182, 0.0));
		}

	}
}