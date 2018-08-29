#ifndef PHYSICS_HARDWARE_ILAUNCHVEHICLE_H
#define PHYSICS_HARDWARE_ILAUNCHVEHICLE_H
#pragma once

#include "IStage.h"

#include <vector>
#include <algorithm>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include <PhysicsFramework/RigidBodyGroup.h>

namespace Physics {
	namespace Hardware {
		
		class ILaunchVehicle : public RigidBodyGroup {
		private:

		public:
			ILaunchVehicle();
			virtual ~ILaunchVehicle() = default;

			void update(double t, double dt);
			
		private:
			virtual void assemble() = 0;
			void basicCollision();

		};

	}
}

#endif
