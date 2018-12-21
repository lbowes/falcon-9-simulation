#ifndef PHYSICS_HARDWARE_GRIDFIN_H
#define PHYSICS_HARDWARE_GRIDFIN_H
#pragma once

#include "Physics/Hardware/Common/Core/StageComponent.hpp"

#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/rotate_vector.hpp>

namespace Graphics {
	class GridFinMesh;
}

namespace Physics {
	class DSS;
}

namespace Physics {
	namespace Hardware {
		
		class GridFin : public StageComponent {
			friend class Graphics::GridFinMesh;
			friend class Physics::DSS;
		private:
			static constexpr glm::dvec2 m2DMountPoint_stage = glm::dvec2(1.97, 40.3);

			static constexpr double
				mRollRange_degs = 90.0,
				mMaxPitch_degs = 0.0,
				mDeployRate = 0.1,
				mStowRate = 0.2,		 //(after landing)
				mMinPitch_degs = -90.0,
				mCmDistFromTop = 0.4064; //m

			const double mClockingDegree_degs = 0.0;
			
			enum class Phase { stowed, deploying, deployed, stowing };
			Phase mCurrentPhase = Phase::stowed;

			double
				mRollAngle = 0.0,	     //vertical = 0, upper bound (local right) = 45, lower bound (local left) = -45, rotational range therefore = 90
				mPitchAngle = -90.0,     //horizontal is 0, upper bound is 0, lower bound is -90, rotational range is therefore 90                                  
				mPercentDeployed = 0.0;  //0.0 = stowed, 1.0 = deployed and locked in place                                                                        

		public:
			GridFin(double clockingDegree_degs);
			~GridFin() = default;

			void update(double dt/* , double fluidDensity, glm::dvec3 flowVelocity_stage */);
			void setRoll(double newRollValue);
			void deploy();
			void stow();
			
			Phase getCurrentPhase() const { return mCurrentPhase; }

		private:
			void updateCompToStage_rotation();

		};

	}
}

#endif
