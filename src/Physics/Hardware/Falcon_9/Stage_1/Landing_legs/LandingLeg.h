#ifndef PHYSICS_HARDWARE_LANDINGLEG_H
#define PHYSICS_HARDWARE_LANDINGLEG_H
#pragma once

#include "Physics/Hardware/Common/Core/IStageComponent.hpp"
#include "TelescopingPiston.h"
#include "LegDeploymentActuator.hpp"
#include "Physics/External/Environment.h"

#include <glm/vec2.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <algorithm>
#include <memory>

namespace Graphics {
	class LandingLegMesh;
}

namespace Physics {
	namespace Hardware {

		class LandingLeg : public IStageComponent {
			friend class Graphics::LandingLegMesh;
		private:
			static constexpr glm::dvec2
				mPos_stage2D = { 1.43521, 0.90617 },        //m
				mPistonStartPos_stage2D = { 2.0, 5.41417 }, //m
				mPistonEndPos_leg2D = { 0.56479, 8.7 },     //m
				mCentreMassPos_leg2D = { 0.4476, 4.52721 }, //m
				mPusherEndPos_leg2D = { 0.88117, 3.80921 }; //m
			
			static constexpr double mRestitutionCoeff = 0.2;
			
			enum class Phase { stowed, deploying, locked };
			Phase mDeploymentPhase = Phase::stowed;

			std::unique_ptr<TelescopingPiston> mPiston;
			std::unique_ptr<LegDeploymentActuator> mPusher;
			
			const glm::dvec3
				mPistonStartPos_stage3D,
				mPusherStartPos_stage3D;

			glm::dvec3
				mAlongPiston_stage3D,
				mPistonEndPos_stage3D,
				mAlongPusher_stage3D,
				mPusherEndPos_stage3D;
												 
			const double mClockingDegree_degs = 0.0;
			
			double
				mDeploymentAngle_rads = 0.0,
				mDeploymentVelocity_rads = 0.0;	 

		public:
			LandingLeg(double clockingDegree_degs);
			~LandingLeg() = default;

			void update(const CoordTransform3D& stageToWorld, glm::dvec3 legOriginAccel_world, double dt);
			void deploy();
			void stow_temp();

			TelescopingPiston* getPiston() const { return mPiston.get(); }

		private:
			void updateState(const CoordTransform3D& stageToWorld, glm::dvec3 legOriginAccel_world, double dt);
			void updateCompToStage_rotation();
			void clampRotationRange(double maxAngle);

		};

	}
}

#endif
