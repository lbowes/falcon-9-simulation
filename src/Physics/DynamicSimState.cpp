#include "DynamicSimState.h"

namespace Physics {

	DynamicSimState::DynamicSimState(const std::string& dataString) 
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

		//Initialise members with the correct data
		mSimulationTime = dataPoints[0];

		F9.RB.CoMPosition_world = { dataPoints[], dataPoints[], dataPoints[] };
		F9.RB.velocity =          { dataPoints[], dataPoints[], dataPoints[] };         
		F9.RB.acceleration =      { dataPoints[], dataPoints[], dataPoints[] };     
		F9.RB.momentum =          { dataPoints[], dataPoints[], dataPoints[] };         
		F9.RB.orientation =       { dataPoints[], dataPoints[], dataPoints[], dataPoints[] };      
		F9.RB.angularVelocity =   { dataPoints[], dataPoints[], dataPoints[] };
		F9.RB.angularMomentum =   { dataPoints[], dataPoints[], dataPoints[] }; 
		F9.RB.CoMPosition_local = { dataPoints[], dataPoints[], dataPoints[] };
		F9.RB.mass =                dataPoints[];
		F9.RB.localToWorld.setLocalToParent_translation(F9.RB.CoMPosition_world);
		F9.RB.localToWorld.setLocalToParent_rotation(F9.RB.orientation);

		//F9.S1.RB -> "Falcon 9 Stage 1 Rigid body"
		F9.S1.RB.CoMPosition_world =  { dataPoints[],  dataPoints[],  dataPoints[] };
		F9.S1.RB.velocity =           { dataPoints[],  dataPoints[],  dataPoints[] };
		F9.S1.RB.acceleration =       { dataPoints[],  dataPoints[],  dataPoints[] };
		F9.S1.RB.momentum =           { dataPoints[], dataPoints[], dataPoints[] };
		F9.S1.RB.orientation =        { dataPoints[], dataPoints[], dataPoints[], dataPoints[] };
		F9.S1.RB.angularVelocity =    { dataPoints[], dataPoints[], dataPoints[] };
		F9.S1.RB.angularMomentum =    { dataPoints[], dataPoints[], dataPoints[] };
		F9.S1.RB.CoMPosition_local =  { dataPoints[], dataPoints[], dataPoints[] };
		F9.S1.RB.mass =                 dataPoints[];
		F9.S1.RB.localToWorld.setLocalToParent_translation(F9.S1.RB.CoMPosition_world);
		F9.S1.RB.localToWorld.setLocalToParent_rotation(F9.S1.RB.orientation);

		F9.S1.LOXMass =               dataPoints[];
		F9.S1.RP1Mass =               dataPoints[];
		F9.S1.nitrogenMass =          dataPoints[];

		//TODO: Complete simulation state for stage 1

		//TODO: Add state for stage 2 (and things like environment?)	
	}

	DynamicSimState DynamicSimState::lerp(const DynamicSimState& a, const DynamicSimState& b, double x) {
		DynamicSimState output;

		output.mSimulationTime = (a.mSimulationTime, b.mSimulationTime, x);

		output.F9.S1.RB.CoMPosition_world = glm::lerp(a.F9.S1.RB.CoMPosition_world, b.F9.S1.RB.CoMPosition_world, x);    
		output.F9.S1.RB.velocity =          glm::lerp(a.F9.S1.RB.velocity, b.F9.S1.RB.velocity, x);
		output.F9.S1.RB.acceleration =      glm::lerp(a.F9.S1.RB.acceleration, b.F9.S1.RB.acceleration, x);
		output.F9.S1.RB.momentum =          glm::lerp(a.F9.S1.RB.momentum, b.F9.S1.RB.momentum, x);    
		output.F9.S1.RB.orientation =       glm::lerp(a.F9.S1.RB.orientation, b.F9.S1.RB.orientation, x);
		output.F9.S1.RB.angularVelocity =   glm::lerp(a.F9.S1.RB.angularVelocity, b.F9.S1.RB.angularVelocity, x);
		output.F9.S1.RB.angularMomentum =   glm::lerp(a.F9.S1.RB.angularMomentum, b.F9.S1.RB.angularMomentum, x);
		output.F9.S1.RB.mass =              lerp(a.F9.S1.RB.mass, b.F9.S1.RB.mass, x);
		output.F9.S1.RB.localToWorld.setLocalToParent_translation(output.F9.S1.RB.CoMPosition_world);
		output.F9.S1.RB.localToWorld.setLocalToParent_rotation(output.F9.S1.RB.orientation);

		output.F9.S1.LOXMass =              lerp(a.F9.S1.LOXMass, b.F9.S1.LOXMass, x);
		output.F9.S1.RP1Mass =              lerp(a.F9.S1.RP1Mass, b.F9.S1.RP1Mass, x);
		output.F9.S1.nitrogenMass =         lerp(a.F9.S1.nitrogenMass, b.F9.S1.nitrogenMass, x);

		//TODO: Complete simulation state





		return output;
	}

	double DynamicSimState::lerp(const double& a, const double& b, double x) {
		return a + (b - a) * x;
	}

}