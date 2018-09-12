//This class should contain one instance of every piece of **dynamic** simulation state.

#ifndef PHYSICS_SIMSTATE_H 
#define PHYSICS_SIMSTATE_H
#pragma once

#include <glm/vec3.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/compatibility.hpp>
#include <string>
#include <sstream>
#include <vector>

namespace Physics {

	struct RigidBodyState {
		glm::dvec3
			CoMPosition_world = glm::dvec3(0.0),
			velocity = glm::dvec3(0.0),
			acceleration = glm::dvec3(0.0),
			momentum = glm::dvec3(0.0),
			angularVelocity = glm::dvec3(0.0),
			angularMomentum = glm::dvec3(0.0),
			CoMPosition_local = glm::dvec3(0.0);

		glm::dquat orientation = glm::dquat();

		double mass = 0.0;
	};

	class SimState {
	public:
		double mSimulationTime = 0.0;

		struct Falcon9 {
			RigidBodyState RB;

			struct Stage1 {
				RigidBodyState RB;

				double 
					LOXMass = 0.0,
					RP1Mass = 0.0,
					nitrogenMass = 0.0;

				//All bools
				//TODO
			}; 
			Stage1 S1;

			struct Stage2 {
				RigidBodyState RB;

				//TODO
			};
			Stage2 S2;
		};
		Falcon9 F9;

	public:
		SimState() = default;
		SimState(const std::string& dataString);
		~SimState() = default;

		static SimState lerp(const SimState& a, const SimState& b, double x);
		static double lerp(const double& a, const double& b, double x);	

	};

}

#endif
