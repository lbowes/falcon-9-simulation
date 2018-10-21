#include "Falcon9.h"

namespace Physics {
	namespace Hardware {

		Falcon9::Falcon9() :
			ILaunchVehicle({ "LC-39A", { 0.0, 0.0, 0.0 } })
		{
			//glm::dmat4 transform = mStage1.getState().getObjectSpace().getLocalToParent_total();
			//printf("-----stage 1 local to world space transform-----\n");
			//printf("%f, %f, %f, %f\n", transform[0][0], transform[0][1], transform[0][2], transform[0][3]);
			//printf("%f, %f, %f, %f\n", transform[1][0], transform[1][1], transform[1][2], transform[1][3]);
			//printf("%f, %f, %f, %f\n", transform[2][0], transform[2][1], transform[2][2], transform[2][3]);
			//printf("%f, %f, %f, %f\n", transform[3][0], transform[3][1], transform[3][2], transform[3][3]);

			assemble();
			
			//glm::dvec3 pos = mStage1.getState().getCoMPosition_world();
			//printf("(Falcon9 ctor) Stage 1 centre of mass position (world space): %f, %f, %f\n", pos.x, pos.y, pos.z);
			//
			//pos = mStage2.getState().getCoMPosition_world();
			//printf("(Falcon9 ctor) Stage 2 centre of mass position (world space): %f, %f, %f\n", pos.x, pos.y, pos.z);
			//
			//std::cin.get();
		}

		void Falcon9::update(double t, double dt) {
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