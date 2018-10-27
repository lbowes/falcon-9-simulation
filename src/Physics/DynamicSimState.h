//This class should contain one instance of every piece of **dynamic** simulation state.

#ifndef PHYSICS_DYNAMICSIMSTATE_H 
#define PHYSICS_DYNAMICSIMSTATE_H
#pragma once

#include "External/SurfaceLocation.h"
#include "PhysicsFramework/CoordTransform3D.h"
#include "PhysicsFramework/RigidBody.h"

#include <glm/vec3.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/compatibility.hpp>
#include <string>
#include <sstream>
#include <vector>

namespace Physics {

	class DSS {
	public:
		struct RigidBodyState {
			glm::dvec3
				CoMPosition_world = glm::dvec3(0.0),
				velocity = glm::dvec3(0.0),
				acceleration = glm::dvec3(0.0),
				momentum = glm::dvec3(0.0),
				angularVelocity = glm::dvec3(0.0),
				angularMomentum = glm::dvec3(0.0);

			glm::dquat orientation = glm::dquat();

			Mass mass_local;
			
			InertiaTensor inertiaTensor_local;

			CoordTransform3D localToWorld;
		};

		struct FluidTankState {
			double
				fluidVolume = 0.0,
				fluidLevel = 0.0,
				percentFull = 0.0;

			Mass 
				mass_comp,
				fluidMass_tank;

			InertiaTensor 
				CoMInertia_comp,
				fluidInertia_fluidCoM;
		};

		struct EngineState {
			double 
				throttle = 0.0,
				thrustMagnitude = 0.0,
				currentMassFlowRate = 0.0;

			glm::dvec3 thrust_stage = glm::dvec3(0.0);
			
			glm::dvec2 gimbalXY = glm::dvec2(0.0);	    

			bool active = false;
		};

		struct GasThrusterState {
			double
				thrustMagnitude = 0.0,
				currentMassFlowRate = 0.0;

			glm::dvec3 thrust_stage = glm::dvec3(0.0);

			bool active = false;
		};

		double simulationTime = 0.0;

		struct Falcon9 {
			RigidBodyState RB;
			
			External::SurfaceLocation inertialPosition;

			struct Stage1 {
				RigidBodyState RB;

				FluidTankState 
					LOXTank,
					RP1Tank,
					nitrogenTank;

				std::vector<EngineState> engines = std::vector<EngineState>(9);
				
				std::vector<GasThrusterState> thrusters = std::vector<GasThrusterState>(8);

				struct LandingLegState {
					enum class Phase : unsigned char { stowed, deploying, locked };
					Phase deploymentPhase = Phase::stowed;
					
					double
						deploymentAngle_rads = 0.0,
						deploymentVelocity_rads = 0.0;

					glm::dvec3
						alongPiston_stage3D = glm::dvec3(0.0),
						pistonEndPos_stage3D = glm::dvec3(0.0),
						alongPusher_stage3D = glm::dvec3(0.0),
						pusherEndPos_stage3D = glm::dvec3(0.0);

					CoordTransform3D compToStage;

					struct TelescopingPistonState {
						double
							currentLength = 0.0,
							lastLength = 0.0,
							extensionRate = 0.0;
					} telescopingPiston;

					struct LegDeploymentActuatorState {
						double
							springLength = 0.0,
							springCompressionRate = 0.0;
					} pusher;
				};
				std::vector<LandingLegState> landingLegs = std::vector<LandingLegState>(4);

				struct GridFinState {
					enum class Phase : unsigned char { stowed, deploying, deployed, stowing };
					Phase currentPhase = Phase::stowed;

					CoordTransform3D compToStage;

					double
						rollAngle = 0.0,
						pitchAngle = -90.0,
						percentDeployed = 0.0;
				};
				std::vector<GridFinState> gridFins = std::vector<GridFinState>(4);
			} S1;

			struct Stage2 {
				RigidBodyState RB;
				
				FluidTankState 
					LOXTank,
					RP1Tank;

				EngineState engine;
			} S2;
		} F9;

	public:
		DSS() = default;
		DSS(const std::string& dataString);
		~DSS() = default;

		//Note: The two functions below do not contain the same parameters, because rigid body acceleration is implicit: it can be saved, but not loaded.
		//Therefore loadRigidBodyState() only requires a State instance, not a RigidBody.
		static void loadRigidBodyState(const DSS::RigidBodyState& source, State& dest);
		static void saveRigidBodyState(const RigidBody& source, DSS::RigidBodyState& dest);

		static DSS lerp(const DSS& a, const DSS& b, double x);

	};

}

#endif
