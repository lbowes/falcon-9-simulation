#include "ILaunchVehicle.h"

namespace Physics {
	namespace Hardware {

		ILaunchVehicle::ILaunchVehicle(const Physics::External::SurfaceLocation& inertialPosition) :
			RigidBodyGroup(Integrator::rungeKutta4, SubUpdateDepth::deep),
			mInertialPosition(inertialPosition)
		{ }

		void ILaunchVehicle::update(double t, double dt) {
			RigidBodyGroup::update(t, dt);
			basicCollision();
		}

		void ILaunchVehicle::basicCollision() {
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