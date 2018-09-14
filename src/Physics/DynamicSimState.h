//This class should contain one instance of every piece of **dynamic** simulation state.

#ifndef PHYSICS_DYNAMICSIMSTATE_H 
#define PHYSICS_DYNAMICSIMSTATE_H
#pragma once

#include "External/SurfaceLocation.h"
#include "PhysicsFramework/CoordTransform3D.h"

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

		CoordTransform3D localToWorld;
	};

	class DynamicSimState {
	public:
		double mSimulationTime = 0.0;

		struct Falcon9 {
			RigidBodyState RB;
			
			CoordTransform3D EUN_to_ECEFTransform;

			struct Stage1 {
				RigidBodyState RB;

				double 
					LOXMass = 0.0,
					RP1Mass = 0.0,
					nitrogenMass = 0.0;

				struct LandingLeg {
					CoordTransform3D legToStage;

				
				} leg1, leg2, leg3, leg4;

				struct GridFin {

				} fin1, fin2, fin3, fin4;
			} S1;

			struct Stage2 {
				RigidBodyState RB;
				
				double 
					LOXMass = 0.0,
					RP1Mass = 0.0,
					nitrogenMass = 0.0;

				//TODO
			} S2;
		} F9;

	public:
		DynamicSimState() = default;
		DynamicSimState(const std::string& dataString);
		~DynamicSimState() = default;

		static DynamicSimState lerp(const DynamicSimState& a, const DynamicSimState& b, double x);
		static double lerp(const double& a, const double& b, double x);	

	};

}

#endif
