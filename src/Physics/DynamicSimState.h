//This class should contain one instance of every piece of **dynamic** simulation state.

#ifndef PHYSICS_DYNAMICSIMSTATE_H 
#define PHYSICS_DYNAMICSIMSTATE_H
#pragma once

#include "External/SurfaceLocation.h"

#include <PhysicsFramework/CoordTransform3D.h>
#include <PhysicsFramework/RigidBody.h>
#include <PhysicsFramework/RigidBodyStateSnapshot.h>
#include <glm/vec3.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/compatibility.hpp>
#include <string>
#include <sstream>
#include <vector>

namespace Physics {
	namespace Hardware {
		class Falcon9;
		class FluidTank;
		class Engine;
		class GasThruster;
		class LandingLeg;
		class GridFin;
		class TelescopingPiston;
		class LegDeploymentActuator;
	}
}

namespace Physics {

	double lerp(const double a, const double b, const double x);

	class DSS {
	public:
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

		struct MerlinEngineState {
			double 
				throttle = 0.0,
				thrustMagnitude = 0.0,
				currentMassFlowRate = 0.0;

			glm::dvec3 thrust_stage = glm::dvec3(0.0);
			
			bool active = false;

			CoordTransform3D compToStage;

			std::vector<double> TVCActuatorAngles = std::vector<double>(2);
		};

		struct GasThrusterState {
			double
				thrustMagnitude = 0.0,
				currentMassFlowRate = 0.0;

			glm::dvec3 thrust_stage = glm::dvec3(0.0);

			bool active = false;
		};

		unsigned int number = 0;

		struct Falcon9 {
			RigidBodyStateSnapshot RB;
			
			struct Stage1 {
				RigidBodyStateSnapshot RB;

				FluidTankState 
					LOXTank,
					RP1Tank,
					nitrogenTank;

				std::vector<MerlinEngineState> engines = std::vector<MerlinEngineState>(9);
				
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
				RigidBodyStateSnapshot RB;
				
				FluidTankState 
					LOXTank,
					RP1Tank;

				MerlinEngineState engine;
			} S2;
		} F9;

	public:
		DSS() = default;
		DSS(const std::string& dataString);
		DSS(const unsigned snapshotNumber, const Hardware::Falcon9& falcon9);
		~DSS() = default;
 
		static void save(const Physics::Hardware::Falcon9& source, DSS& dest);
		static void load(const DSS& source, Physics::Hardware::Falcon9& dest);
		static void lerp(const DSS& a, const DSS& b, double x, DSS& dest);

		static void saveFluidTankState(const Physics::Hardware::FluidTank& source, DSS::FluidTankState& dest);
		static void loadFluidTankState(const DSS::FluidTankState& source, Physics::Hardware::FluidTank& dest);
		static void lerpFluidTankState(const DSS::FluidTankState& a, const DSS::FluidTankState& b, double x, DSS::FluidTankState& dest);

		static void saveEngineState(const Physics::Hardware::Engine& source, DSS::MerlinEngineState& dest);
		static void loadEngineState(const DSS::MerlinEngineState& source, Physics::Hardware::Engine& dest);
		static void lerpEngineState(const DSS::MerlinEngineState& a, const DSS::MerlinEngineState& b, double x, DSS::MerlinEngineState& dest);

		static void saveGasThrusterState(const Physics::Hardware::GasThruster& source, DSS::GasThrusterState& dest);
		static void loadGasThrusterState(const DSS::GasThrusterState& source, Physics::Hardware::GasThruster& dest);
		static void lerpGasThrusterState(const DSS::GasThrusterState& a, const DSS::GasThrusterState& b, double x, DSS::GasThrusterState& dest);

		static void saveLandingLegState(const Physics::Hardware::LandingLeg& source, DSS::Falcon9::Stage1::LandingLegState& dest);
		static void loadLandingLegState(const DSS::Falcon9::Stage1::LandingLegState& source, Physics::Hardware::LandingLeg& dest);
		static void lerpLandingLegState(const DSS::Falcon9::Stage1::LandingLegState& a, const DSS::Falcon9::Stage1::LandingLegState& b, double x, DSS::Falcon9::Stage1::LandingLegState& dest);

		static void saveGridFinState(const Physics::Hardware::GridFin& source, DSS::Falcon9::Stage1::GridFinState& dest);
		static void loadGridFinState(const DSS::Falcon9::Stage1::GridFinState& source, Physics::Hardware::GridFin& dest);
		static void lerpGridFinState(const DSS::Falcon9::Stage1::GridFinState& a, const DSS::Falcon9::Stage1::GridFinState& b, double x, DSS::Falcon9::Stage1::GridFinState& dest);

		static void saveLegPistonState(const Physics::Hardware::TelescopingPiston& source, DSS::Falcon9::Stage1::LandingLegState::TelescopingPistonState& dest);
		static void loadLegPistonState(const DSS::Falcon9::Stage1::LandingLegState::TelescopingPistonState& source, Physics::Hardware::TelescopingPiston& dest);
		static void lerpLegPistonState(
			const DSS::Falcon9::Stage1::LandingLegState::TelescopingPistonState& a,
			const DSS::Falcon9::Stage1::LandingLegState::TelescopingPistonState& b,
			double x, 
			DSS::Falcon9::Stage1::LandingLegState::TelescopingPistonState& dest
		);

		static void saveLegPusherState(const Physics::Hardware::LegDeploymentActuator& source, DSS::Falcon9::Stage1::LandingLegState::LegDeploymentActuatorState& dest);
		static void loadLegPusherState(const DSS::Falcon9::Stage1::LandingLegState::LegDeploymentActuatorState& source, Physics::Hardware::LegDeploymentActuator& dest);
		static void lerpLegPusherState(
			const DSS::Falcon9::Stage1::LandingLegState::LegDeploymentActuatorState& a, 
			const DSS::Falcon9::Stage1::LandingLegState::LegDeploymentActuatorState& b, 
			double x,
			DSS::Falcon9::Stage1::LandingLegState::LegDeploymentActuatorState& dest
		);

	};

}

#endif
