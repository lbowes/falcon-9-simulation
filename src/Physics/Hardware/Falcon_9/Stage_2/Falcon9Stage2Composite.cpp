#include "Falcon9Stage2Composite.h"

namespace Physics {
	namespace Hardware {

		Falcon9Stage2Composite::Falcon9Stage2Composite() :
			RigidBodyGroup(Integrator::rungeKutta4, MemberUpdateDepth::deep)
		{
			//addBody(mStageCore);
			
			const glm::dvec3 fairingPos_stage { 0.0, 12.6, 0.0 };
			//addBody(mFairingHalves.first, {fairingPos_stage, glm::rotate(glm::radians(0.0), glm::dvec3(0.0, 1.0, 0.0))});
			//addBody(mFairingHalves.second, {fairingPos_stage, glm::rotate(glm::radians(180.0), glm::dvec3(0.0, 1.0, 0.0))});
		}

		void Falcon9Stage2Composite::update(double t, double dt) {
			mStageCore.update(t, dt);
			mFairingHalves.first.update(t, dt);
			mFairingHalves.second.update(t, dt);

			//if(t > 35.0)
			//	mStageCore.getEngines().get<Merlin1DVac>(0)->setActive(true);

			//if(t > 40.0) {
			//	mMembers[1].disconnect();
			//	mMembers[2].disconnect();
			//}

			RigidBodyGroup::update(t, dt);
			basicCollision_temp();
		}

		void Falcon9Stage2Composite::basicCollision_temp() {
			glm::dvec3 currentPos_world = mState.getObjectSpace().toParentSpace();
			
			const double groundHeight = 0.0; //3.0

			if (currentPos_world.y < groundHeight) {
				currentPos_world.y += groundHeight - currentPos_world.y;

				mState.setPosition_world(currentPos_world);

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