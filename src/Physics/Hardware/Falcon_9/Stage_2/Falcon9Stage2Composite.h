#ifndef PHYSICS_HARDWARE_FALCON9STAGE2COMPOSITE_H
#define PHYSICS_HARDWARE_FALCON9STAGE2COMPOSITE_H
#pragma once

#include "Falcon9Stage2.h"
#include "FairingHalf.h"
//TODO
//#include "Payload.h"

#include <PhysicsFramework/RigidBodyGroup.h>

namespace Physics {
	namespace Hardware {

		class Falcon9Stage2Composite : public RigidBodyGroup {
		private:
			Falcon9Stage2 mStageCore;

			std::pair<FairingHalf, FairingHalf> mFairingHalves;

		public:
			Falcon9Stage2Composite();
			~Falcon9Stage2Composite() = default;

			void update(double t, double dt);

			const Falcon9Stage2& getCore() const { return mStageCore; }
			Falcon9Stage2& getCore() { return mStageCore; }
			const std::pair<FairingHalf, FairingHalf>& getFairings() const { return mFairingHalves; }
			std::pair<FairingHalf, FairingHalf>& getFairings() { return mFairingHalves; }

		private:
			void basicCollision_temp();

		};

	}
}

#endif
