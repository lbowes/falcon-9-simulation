#include "DynamicSimState.h"

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
		const State& rigidBodyState = source.getState();

		dest.CoMPosition_world = rigidBodyState.getCoMPosition_world();
		dest.velocity = rigidBodyState.getVelocity_world();
		dest.acceleration = source.getAccel_world();
		dest.momentum = rigidBodyState.getMomentum_world();
		dest.orientation = rigidBodyState.getOrientation_world();			
		dest.angularVelocity = rigidBodyState.getAngularVelocity_world();
		dest.angularMomentum = rigidBodyState.getAngularMomentum_world();
		dest.mass_local = rigidBodyState.getMass_local();
		dest.inertiaTensor_local = rigidBodyState.getInertiaTensor_local();
		dest.localToWorld = rigidBodyState.getObjectSpace();
	}

	DSS DSS::lerp(const DSS& a, const DSS& b, double x) {
		DSS output;

		//TODO: Linearly interpolate between two states

		return output;
	}

}