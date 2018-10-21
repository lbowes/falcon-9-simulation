#ifndef PHYSICS_HARDWARE_FALCONLANDINGLEGS_H
#define PHYSICS_HARDWARE_FALCONLANDINGLEGS_H
#pragma once

#include "LandingLeg.h"
#include "Physics/Hardware/Common/Core/StageComponentGroup.h"
#include "Physics/DynamicSimState.h"

#include <GraphicsFramework/Input.h>
#include <PhysicsFramework/RigidBody.h>

namespace Physics {
	namespace Hardware {

		class LandingLegs : public StageComponentGroup {
		public:
			LandingLegs();
			~LandingLegs() = default;

			void update(const RigidBody& stage, double dt);
			void loadDynamicState(const DSS::Falcon9::Stage1& stage1State);
			void saveDynamicState(DSS::Falcon9::Stage1& toSaveTo) const;

		private:
			void addLegs();
			void updateAllLegs(const RigidBody& stage, double dt) const;

		};

	}
}

#endif
