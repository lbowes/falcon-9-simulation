#include "Falcon9.h"

namespace Physics {
	namespace Hardware {

		Falcon9::Falcon9() :
			ILaunchVehicle({ "LC-39A", {0.0, 0.0, 0.0}})
		{
			assemble();
		}

		void Falcon9::update(double t, double dt) {
			mStage1.update(t, dt);
			mStage2.update(t, dt);

			if(t > 60.0)
				mMembers[1].disconnect();

			ILaunchVehicle::update(t, dt);
		}

		void Falcon9::assemble() {
			addBody(mStage1);
			addBody(mStage2, glm::dvec3(0.0, 45.76182, 0.0));
		}

	}
}