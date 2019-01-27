#ifndef PHYSICS_HARDWARE_LANDINGLEG_H
#define PHYSICS_HARDWARE_LANDINGLEG_H

#include "../../Common/Structural/IStageComponent.hpp"

#include <core/ChVector2.h>
#include <chrono/physics/ChBodyAuxRef.h>
#include <chrono/physics/ChLinkRevolute.h>
#include <memory>

namespace chrono {
	class ChSystem;
}

namespace Physics {
	namespace Hardware {

		class LandingLeg {
		private:
			// Position (m) of the A-frame joint, w.r.t the stage's origin, in 2D.
			static constexpr chrono::ChVector2 sPos_stage2D = {1.43521, 0.90617};
				
			// Position (m) of the telescoping piston joint (roughly half way up the RP-1 tank), w.r.t the stage's origin in 2D.
			static constexpr chrono::ChVector2 sPistonStartPos_stage2D = {2.04213, 5.30092};
			
			// Position (m) of the deployment actuator joint (just beneath sPistonStartPos_stage2D), w.r.t the stage's origin in 2D.
			static constexpr chrono::ChVector2 sPusherStartPos_stage2D = {1.80105, 5.20809};

			// Position (m) of the telescoping piston's connection point at the tip of the A-frame, w.r.t the A-Frame's origin in 2D.
			static constexpr chrono::ChVector2 sPistonEndPos_AFrame2D = {0.575, 8.7};

			// Position (m) of the A-frame CoM, w.r.t the A-Frame's origin in 2D.
			static constexpr chrono::ChVector2 sCoMPos_AFrame2D = {0.4476, 4.52721};

			// Position (m) of the deployment actuator's contact point on the A-frame, w.r.t the A-Frame's origin in 2D.
			static constexpr chrono::ChVector2 sPusherEndPos_AFrame2D = {0.85, 3.9};

			// Mass (kg) of the A-frame
			static constexpr double sAFrameMass = 600.0;

			// Number of landing legs currently instantiated
			static unsigned char sInstanceCount;

			// The angle (degs) of a leg system around the stage, taken from the +X axis. 
			const double mClockingDegree;

			chrono::ChBodyAuxRef& mStage1Body;

			chrono::ChSystem* mSystemHandle;

			std::shared_ptr<chrono::ChBodyAuxRef> mAFrameBody;

			std::shared_ptr<chrono::ChLinkLockRevolute> mAFrameJoint;

			enum class Phase : unsigned char { stowed, deploying, locked };
			Phase mDeploymentPhase = Phase::stowed;

		public:
			LandingLeg(chrono::ChBodyAuxRef& stage1Body, double clockingDegree);
			~LandingLeg() = default;

		private:
			void attachToStage();
			chrono::ChMatrix33<> calcInertia_AFrame() const;	

		};

	}
}

#endif
