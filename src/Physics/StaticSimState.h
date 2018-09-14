//This class should be responsible for storing the state in the simulation that does not change. It should be passed down to the objects in the simulation, in chunks,
//so the objects can be initialised with the correct data before the simulation starts.
//This class will also be passed to the Visualisation.
//Data should be loaded from a JSON file. 

#ifndef PHYSICS_STATICSIMSTATE_H
#define PHYSICS_STATICSIMSTATE_H
#pragma once

#include "External/SurfaceLocation.h"

#include <string>

namespace Physics {

	class StaticSimState {
	public:
		struct Falcon9 {
			External::SurfaceLocation mInertialPosition;
			
			struct Stage1 {
				const double 
					height = 0.0, //TODO
					diameter = 0.0, //TODO
					inertMass = 0.0; //TODO

				struct LandingLegs {
					const unsigned char
						count = 4,
						pistonCylinderCount = 5;

				} landingLegs;

			} S1;

			struct Stage2 {
				const double 
					height = 0.0, //TODO
					diameter = 0.0, //TODO
					inertMass = 0.0; //TODO;
			} S2;
		} F9;

	public:
		StaticSimState(const std::string& jsonDataFilePath);
		~StaticSimState() = default;

	};

}

#endif
