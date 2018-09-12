#include "SimState.h"

namespace Physics {

	SimState::SimState(const std::string& dataString) 
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

		//Initialise the data members with the correct items
		mSimulationTime = dataPoints[0];

		//TODO:
		//Add rigid body state for Falcon9 (composite body)

		//F9.S1.RB -> "Falcon 9 Stage 1 Rigid body"
		F9.S1.RB.CoMPosition_world = { dataPoints[1],  dataPoints[2],  dataPoints[3]                  };
		F9.S1.RB.velocity =          { dataPoints[4],  dataPoints[5],  dataPoints[6]                  };
		F9.S1.RB.acceleration =      { dataPoints[7],  dataPoints[8],  dataPoints[9]                  };
		F9.S1.RB.momentum =          { dataPoints[10], dataPoints[11], dataPoints[12]                 };
		F9.S1.RB.orientation =       { dataPoints[13], dataPoints[14], dataPoints[15], dataPoints[16] };
		F9.S1.RB.angularVelocity =   { dataPoints[17], dataPoints[18], dataPoints[19]                 };
		F9.S1.RB.angularMomentum =   { dataPoints[20], dataPoints[21], dataPoints[22]                 };

		F9.S1.RB.mass =              dataPoints[23];
		F9.S1.LOXMass =              dataPoints[24];
		F9.S1.RP1Mass =              dataPoints[25];
		F9.S1.nitrogenMass =         dataPoints[26];

		//TODO: Complete simulation state for stage 1

		//TODO: Add state for stage 2 (and things like environment?)	
	}

	SimState SimState::lerp(const SimState& a, const SimState& b, double x) {
		SimState output;

		output.mSimulationTime = (a.mSimulationTime, b.mSimulationTime, x);

		output.F9.S1.RB.CoMPosition_world = glm::lerp(a.F9.S1.RB.CoMPosition_world, b.F9.S1.RB.CoMPosition_world, x);    
		output.F9.S1.RB.velocity =          glm::lerp(a.F9.S1.RB.velocity, b.F9.S1.RB.velocity, x);
		output.F9.S1.RB.acceleration =      glm::lerp(a.F9.S1.RB.acceleration, b.F9.S1.RB.acceleration, x);
		output.F9.S1.RB.momentum =          glm::lerp(a.F9.S1.RB.momentum, b.F9.S1.RB.momentum, x);    
		output.F9.S1.RB.orientation =       glm::lerp(a.F9.S1.RB.orientation, b.F9.S1.RB.orientation, x);
		output.F9.S1.RB.angularVelocity =   glm::lerp(a.F9.S1.RB.angularVelocity, b.F9.S1.RB.angularVelocity, x);
		output.F9.S1.RB.angularMomentum =   glm::lerp(a.F9.S1.RB.angularMomentum, b.F9.S1.RB.angularMomentum, x);

		output.F9.S1.RB.mass =              lerp(a.F9.S1.RB.mass, b.F9.S1.RB.mass, x);
		output.F9.S1.LOXMass =              lerp(a.F9.S1.LOXMass, b.F9.S1.LOXMass, x);
		output.F9.S1.RP1Mass =              lerp(a.F9.S1.RP1Mass, b.F9.S1.RP1Mass, x);
		output.F9.S1.nitrogenMass =         lerp(a.F9.S1.nitrogenMass, b.F9.S1.nitrogenMass, x);

		//TODO: Complete simulation state





		return output;
	}

	double SimState::lerp(const double& a, const double& b, double x) {
		return a + (b - a) * x;
	}

}