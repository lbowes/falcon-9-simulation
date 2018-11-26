#include "DynamicSimState.h"

#include <iostream>

namespace Physics {

	DSS::DSS(const std::string& dataString) 
		//Parses data string to fully populate the member state
	{
		//Parse the data string into a vector of items
		std::vector<double> dataPoints;
		std::stringstream ss(dataString);
		double d;

		while(ss >> d) {
			dataPoints.push_back(d);

			if(ss.peek() == ',')
				ss.ignore();
		}

		//TODO: Fully initialise the dynamic state using the string passed in	
	}

	void DSS::loadRigidBodyState(const DSS::RigidBodyState& source, State& dest) {
		dest.setCoMPosition_world(source.CoMPosition_world);
		dest.setVelocity_world(source.velocity);
		dest.setMomentum_world(source.momentum);
		dest.setOrientation_world(source.orientation);
		dest.setAngularVelocity_world(source.angularVelocity);
		dest.setAngularMomentum_world(source.angularMomentum);
		dest.setMass_local(source.mass_local);
		dest.setInertiaTensor_local(source.inertiaTensor_local);
		dest.setObjectToParentTransform(source.localToWorld);
	}

	void DSS::saveRigidBodyState(const RigidBody& source, DSS::RigidBodyState& dest) {
		const State& s = source.getState();

		dest.CoMPosition_world = s.getCoMPosition_world();
		dest.velocity = s.getVelocity_world();
		dest.acceleration = source.getAccel_world();
		dest.momentum = s.getMomentum_world();
		dest.orientation = s.getOrientation_world();			
		dest.angularVelocity = s.getAngularVelocity_world();
		dest.angularMomentum = s.getAngularMomentum_world();
		dest.mass_local = s.getMass_local();
		dest.inertiaTensor_local = s.getInertiaTensor_local();
		dest.localToWorld = s.getObjectSpace();
	}

	void DSS::lerpRigidBodyState(const DSS::RigidBodyState& a, const DSS::RigidBodyState& b, double x, DSS::RigidBodyState& dest) {
		dest.CoMPosition_world = glm::lerp(a.CoMPosition_world, b.CoMPosition_world, x);
		dest.velocity = glm::lerp(a.velocity, b.velocity, x);
		dest.acceleration = glm::lerp(a.acceleration, b.acceleration, x);
		dest.momentum = glm::lerp(a.momentum, b.momentum, x);
		dest.orientation = glm::slerp(a.orientation, b.orientation, x);
		dest.angularVelocity = glm::lerp(a.angularVelocity, b.angularVelocity, x);
		dest.angularMomentum = glm::lerp(a.angularMomentum, b.angularMomentum, x);
		dest.mass_local = ::lerp(a.mass_local, b.mass_local, x);
		dest.localToWorld = ::lerp(a.localToWorld, b.localToWorld, x);

		//The inertia tensor cannot be interpolated without first interpolating all components that influence it,
		//so the most recent snapshot is used
		dest.inertiaTensor_local = a.inertiaTensor_local;
	}

	//TODO:
	//Work on fully interpolating the state of the vehicle (not just rigid body state).
	//Landing legs, grid fins, propellant, engines
	//Look at how state is propagated down with loading and saving and mirror this passing-down of data in the interpolation

	void DSS::lerp(const DSS& a, const DSS& b, double x, DSS& dest) {
		lerpRigidBodyState(a.F9.RB, b.F9.RB, x, dest.F9.RB);
		lerpRigidBodyState(a.F9.S1.RB, b.F9.S1.RB, x, dest.F9.S1.RB);
		lerpRigidBodyState(a.F9.S2.RB, b.F9.S2.RB, x, dest.F9.S2.RB);
	}

}