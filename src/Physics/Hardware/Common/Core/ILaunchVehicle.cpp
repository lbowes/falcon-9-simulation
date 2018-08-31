#include "ILaunchVehicle.h"

namespace Physics {
	namespace Hardware {

		ILaunchVehicle::ILaunchVehicle(const Physics::External::SurfaceLocation& surfaceLocation) :
			RigidBodyGroup(Integrator::rungeKutta4, SubUpdateDepth::deep),
			mSurfaceLocation(surfaceLocation)
		{ 
			//TODO: -----------------------------------------------------------------------------------------------------------------------------------------------------------------
			//Using the GPS coordinate and the ENU orientation passed in, we need to initialise the inertial transform of the launch vehicle.


			//Consider creating a 'LaunchSite' class, or 'SurfaceLocation'/'EarthLocation' (to accommodate landing sites) and passing in an instance of one of these
			//into the launch vehicle instead of a raw GPS position.
			//The SurfaceLocation can be created with a name and a raw GPS position. It can contain it's own GF::CoordTransform3D representing the ENU frame that can
			//be calculated independently.
			//Then this is passed into this function, as well as used as the earth model camera origin etc.
			//We could then have a separate transform to move the launch vehicle around the launch site perhaps.

			//For example:
			//		External::SurfaceLocation mLC_39A = { "Historic Pad 39A", External::GPSCoord(blah, blah, blah)};

			//We could then have LaunchSite classes full of ground equipment
		}

		void ILaunchVehicle::update(double t, double dt) {
			RigidBodyGroup::update(t, dt);
			basicCollision();
		}

		void ILaunchVehicle::basicCollision() {
			glm::dvec3 currentPos_world = mState.getObjectSpace().toParentSpace();
			
			const double groundHeight = 3.0; //3.0

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