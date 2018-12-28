#ifndef PHYSICS_HARDWARE_FALCONLANDINGLEGS_H
#define PHYSICS_HARDWARE_FALCONLANDINGLEGS_H
#pragma once

#include "LandingLeg.h"
#include "Physics/Hardware/Common/Core/StageComponentGroup.h"

#include <GraphicsFramework/Input.h>
#include <PhysicsFramework/RigidBody.h>

namespace Physics {
	namespace Hardware {

		class LandingLegs : public StageComponentGroup {
		public:
			LandingLegs();
			~LandingLegs() = default;

			void update(const RigidBody& stage, double dt);

		private:
			void updateAllLegs(const RigidBody& stage, double dt) const;

		};

	}
}

#endif
