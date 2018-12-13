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
	class DSS;
}

namespace Physics {
	namespace Hardware {

		class LandingLeg : public IStageComponent {
			friend class Graphics::LandingLegMesh;
			friend class Physics::DSS;
		private:
			static constexpr double mRestitutionCoeff = 0.24;   //0.2

			static constexpr glm::dvec2
				mPos_stage2D = { 1.43521, 0.90617 },            //m
				mPistonStartPos_stage2D = { 2.04213, 5.30092 }, //m (2.0, 5.41417)
				mPusherStartPos_stage2D = { 1.80105, 5.20809 },
				mPistonEndPos_leg2D = { 0.575, 8.7 },           //m (0.56479, 8.7)
				mCentreMassPos_leg2D = { 0.4476, 4.52721 },     //m
				mPusherEndPos_leg2D = { 0.85, 3.9 };            //m (0.88117, 3.80921)
			
			const double mClockingDegree = 0.0;

			const glm::dmat4 mClockingRotation_stage = glm::dmat4(1.0);

			const glm::dvec3
				mPistonStartPos_stage3D,
				mPusherStartPos_stage3D;

			const InertiaTensor mAboutOrigin;

			std::unique_ptr<TelescopingPiston> mPiston;
			std::unique_ptr<LegDeploymentActuator> mPusher;
			
			enum class Phase : unsigned char { stowed, deploying, locked };
			Phase mDeploymentPhase = Phase::stowed;

			double
				mDeploymentAngle_rads = 0.0,
				mDeploymentVelocity_rads = 0.0;

			glm::dvec3
				mAlongTPiston_stage3D,
				mPistonEndPos_stage3D,
				mAlongPusher_stage3D,
				mPusherEndPos_stage3D;

		public:
			LandingLeg(double clockingDegree);
			~LandingLeg() = default;

			void update(const CoordTransform3D& stageToWorld, glm::dvec3 legOriginAccel_world, double dt);
			void deploy();
			
			glm::dvec3 getPistonStartPoint_stage() const { return mPistonStartPos_stage3D; }
			glm::dvec3 getPusherStartPoint_stage() const { return mPusherStartPos_stage3D; }
			TelescopingPiston* getPiston() const { return mPiston.get(); }
			LegDeploymentActuator* getDeploymentActuator() const { return mPusher.get(); }

		private:
			Mass recalcMass_local() const;
			InertiaTensor recalcInertia_local() const;
			void updateState(const CoordTransform3D& stageToWorld, glm::dvec3 legOriginAccel_world, double dt);
			void updateDeploymentActuator();
			void updateCompToStage_rotation();
			void clampRotationRange(double maxAngle);

		};

	}
}

#endif
