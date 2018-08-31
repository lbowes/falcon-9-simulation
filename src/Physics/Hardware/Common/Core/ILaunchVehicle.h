#ifndef PHYSICS_HARDWARE_ILAUNCHVEHICLE_H
#define PHYSICS_HARDWARE_ILAUNCHVEHICLE_H
#pragma once

#include "IStage.h"
#include "Physics/External/SurfaceLocation.h"

#include <vector>
#include <algorithm>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include <PhysicsFramework/RigidBodyGroup.h>

namespace Physics {
	namespace Hardware {
		
		class ILaunchVehicle : public RigidBodyGroup {
		protected:
			Physics::External::SurfaceLocation mSurfaceLocation;

		public:
			ILaunchVehicle(const Physics::External::SurfaceLocation& surfaceLocation);
			virtual ~ILaunchVehicle() = default;

			void update(double t, double dt);
			
			Physics::External::SurfaceLocation& getSurfaceLocation() { return mSurfaceLocation; } 

		private:
			virtual void assemble() = 0;
			void basicCollision();

		};

	}
}

#endif
